#include <pthread.h>
#include <string.h>
#include <stdio.h>

#include "test_ov2640.h"

GPIO_TypeDef spi_cs_port;
GPIO_InitTypeDef spi_cs_init;
uint16_t spi_cs_pin = 0;
SPI_HandleTypeDef spi_handler;
I2C_HandleTypeDef i2c_handler;

#define FIFO_BUFFER_SIZE 100

uint8_t regs[256];
uint8_t fifo_buffer[FIFO_BUFFER_SIZE] = {5};
volatile uint8_t fifo_size1, fifo_size2, fifo_size3;
volatile uint8_t use_camera = 0;

// Thread function that mocks OV2640 camera I2C from the slave end
void * ov2640_i2c_handler(void * arg) {
    while(use_camera == 1) {
        // I2C: Will either write a value to a reg (msg size of 2) or read from a requested reg (msg size of 1)
        if(i2c_handler.State == HAL_I2C_STATE_BUSY_TX) {
            // Correct slave address for OV2640 must be given
            if(i2c_handler.XferAddress == 0x60) {
                // Register write
                if(i2c_handler.MsgSize == 2) {
                    // Receive data of register data pair from I2C master
                    uint8_t reg_data[2];
                    
                    Mock_I2C_Slave_Receive(&i2c_handler, reg_data, 2, HAL_MAX_DELAY);

                    // Write to regs accordingly
                    uint8_t reg = reg_data[0];
                    uint8_t data = reg_data[1];
                    regs[reg] = data;
                }
            }
        }
    }
}

// Thread function that mocks OV2640 camera SPI from the slave end
void * ov2640_spi_handler(void * arg) {
    while(use_camera == 1) {
        // SPI: Do things based on the received messages (we could also write to the registers like with I2C, but we're more interested in the resulting actions)
        if(spi_handler.State == HAL_SPI_STATE_BUSY_TX) {
            // CS pin must be pulled low
            if(HAL_GPIO_ReadPin(&spi_cs_port, spi_cs_pin) == GPIO_PIN_RESET) {
                // Receive command from SPI master and do things based on that
                if(spi_handler.TxMsgSize == 1) {
                    uint8_t cmd;
                    Mock_SPI_Slave_Receive(&spi_handler, &cmd, 1, HAL_MAX_DELAY);

                    // Burst read from the FIFO buffer
                    if(cmd == OV2640_FIFO_BURST_READ) {
                        // Transfer FIFO buffer to master in chunks that it requests
                        // Should terminate if ov2640_clear_fifo is called
                        uint16_t fifo_index = 0;
                        while(fifo_index < FIFO_BUFFER_SIZE) {
                            if(spi_handler.RxMsgSize > 0) {
                                uint16_t to_transmit = spi_handler.RxMsgSize;
                                Mock_SPI_Slave_Transmit(&spi_handler, &fifo_buffer[fifo_index], to_transmit, HAL_MAX_DELAY);
                                fifo_index += to_transmit;
                            }
                        }
                    }
                    // Write (determined based on set write bit at 0x80)
                    else if(cmd & 0x80) {
                        // Register to write to
                        uint8_t reg = cmd & 0x7F;
                        
                        // Receive the data to write to the register
                        uint8_t data;
                        Mock_SPI_Slave_Receive(&spi_handler, &data, 1, HAL_MAX_DELAY);

                        // Do stuff based on register/data writing

                        // Clear FIFO buffer
                        if(reg == OV2640_FIFO_CONTROL && data == OV2640_FIFO_CLEAR_MASK) {
                            for(int i = 0; i < FIFO_BUFFER_SIZE; i++) {
                                fifo_buffer[i] = 0;
                            }

                            // Set FIFO length to 0 (size1 is LSB)
                            fifo_size1 = 0;
                            fifo_size2 = 0;
                            fifo_size3 = 0;
                        }
                        // Start FIFO capture (fill with dummy data)
                        else if(reg == OV2640_FIFO_CONTROL && data == OV2640_FIFO_START_MASK) {
                            for(int i = 0; i < 256; i++) {
                                fifo_buffer[i] = i;
                            }

                            // Set FIFO length to FIFO_BUFFER_SIZE (size1 is LSB)
                            fifo_size1 = (FIFO_BUFFER_SIZE >> 0) & 0xFF;
                            fifo_size2 = (FIFO_BUFFER_SIZE >> 8) & 0xFF;
                            fifo_size3 = (FIFO_BUFFER_SIZE >> 16) & 0xFF;
                        }
                        // Resetting CPLD (I don't think it's necessary to implement in a mock)
                        else if(reg == OV2640_CPLD_REG) {
                            // Do nothing
                        }
                        else if(reg == OV2640_CPLD_RESET) {
                            // Do nothing
                        }

                        // SPI message processed, master can now initiate another transaction
                        spi_handler.State = HAL_SPI_STATE_READY;
                    }
                    // Read (determined based on cleared write bit at 0x80)
                    else if(!(cmd & 0x80)) {
                        // Register to read from
                        uint8_t reg = cmd;

                        // Register data to send back to master
                        uint8_t data;

                        // Mocking the register read
                        if(reg == OV2640_FIFO_SIZE1) {
                            data = fifo_size1;
                        }
                        else if(reg == OV2640_FIFO_SIZE2) {
                            data = fifo_size2;
                        }
                        else if(reg == OV2640_FIFO_SIZE3) {
                            data = fifo_size3;
                        }
                        else if(reg == OV2640_CAPTURE_TRIGGER) {
                            if(fifo_size1 == 0 && fifo_size2 == 0 && fifo_size3 == 0) {
                                data = 0;
                            }
                            else {
                                data = OV2640_CAPTURE_DONE_MASK;
                            }
                        }

                        // Send the register data back to the master
                        Mock_SPI_Slave_Transmit(&spi_handler, &data, 1, HAL_MAX_DELAY);
                    }
                }
            }
        }
    }
}

void ov2640_usage_test() {
    // Initialize the mock GPIO, SPI and I2C handlers
    HAL_Init();
    HAL_StatusTypeDef gpio_init_status = HAL_GPIO_Init(&spi_cs_port, &spi_cs_init);
    HAL_StatusTypeDef spi_init_status = HAL_SPI_Init(&spi_handler);
    HAL_StatusTypeDef i2c_init_status = HAL_I2C_Init(&i2c_handler);

    // Create camera inst and register handlers to it
    ov2640 camera;
    ov2640_register(&camera, &spi_cs_port, spi_cs_pin, &spi_handler, &i2c_handler);

    // Set the use_camera flag to 0 to start the threads
    use_camera = 1;
    // Create a thread to mock the camera
    pthread_t mock_thread1, mock_thread2;
    pthread_create(&mock_thread1, NULL, ov2640_i2c_handler, NULL);
    pthread_create(&mock_thread2, NULL, ov2640_spi_handler, NULL);

    // Initialize camera and set resolution
    ov2640_jpeg_init(&camera);
    ov2640_jpeg_set_res(&camera, OV2640_RES_320x240);

    // Keep trying until a valid capture is taken.
	while(camera.fifo_length == 0) {
	  ov2640_get_capture(&camera);
	}

    // Transfer and print out capture data, one buffer at a time.
	ov2640_transfer_start(&camera);

	// Buffer approach is done for the case where there isn't enough memory to hold the entire image at once.
    uint16_t buffer_length = 10;

    // A buffer holding all FIFO data is used for easier debugging
    uint8_t camera_data[FIFO_BUFFER_SIZE];
    uint16_t camera_data_index = 0;

    // Transfer all data from the camera FIFO to the camera_data buffer
	while(camera.fifo_length > 0) {
		uint16_t buffer_filled;

		ov2640_transfer_step(&camera, &camera_data[camera_data_index], buffer_length, &buffer_filled);

        camera_data_index += buffer_filled;
	}

	ov2640_transfer_stop(&camera);

    // Set the use_camera flag to 1 to terminate the threads
    use_camera = 0;

    // Wait for threads to terminate
    pthread_join(mock_thread1, NULL);
    pthread_join(mock_thread2, NULL);

    // Should have received camera data iterating up to from 0 to (FIFO_BUFFER_SIZE-1)
    uint8_t test_result = 0;
    for(int i = 0; i < FIFO_BUFFER_SIZE; i++) {
        if(camera_data[i] != i) {
            test_result = 1;
        }
    }

    if(test_result == 0) {
        printf("Camera data received correctly\n");
    }
    else {
        printf("Camera data received incorrectly\n");

        for(int i = 0; i < FIFO_BUFFER_SIZE; i++) {
            printf("%d ", camera_data[i]);
        }
        printf("\n");
    }
}

void run_ov2640_tests(void) {
    // I didn't want to deal with stack errors so this test is done without cmocka
    ov2640_usage_test();
}