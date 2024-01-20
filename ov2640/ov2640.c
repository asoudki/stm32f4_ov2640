#include "ov2640.h"
#include "ov2640_regs.h"

// Register all essential port/pin and handler data to perform OV2640 functions
// Additionally 
void ov2640_register(ov2640 *camera, GPIO_TypeDef *spi_cs_port, uint16_t spi_cs_pin, SPI_HandleTypeDef * spi_handler, I2C_HandleTypeDef * i2c_handler) {
    camera->spi_cs_port = spi_cs_port;
    camera->spi_cs_pin = spi_cs_pin;
    camera->spi_handler = spi_handler;
    camera->i2c_handler = i2c_handler;
}

// Select the SPI of OV2640 (active-low)
void ov2640_spi_select(ov2640 *camera) {
    HAL_GPIO_WritePin(camera->spi_cs_port, camera->spi_cs_pin, GPIO_PIN_RESET);  // Active low select
}

// Deselect the SPI of OV2640 (active-low)
void ov2640_spi_deselect(ov2640 *camera) {
    HAL_GPIO_WritePin(camera->spi_cs_port, camera->spi_cs_pin, GPIO_PIN_SET);  // Active low deselect
}

// Writes a specified byte of data to the OV2640 FIFO buffer through SPI
void ov2640_fifo_write(ov2640 *camera, uint8_t addr, uint8_t data) {
    ov2640_spi_select(camera);
    HAL_Delay(10);
    addr |= 0x80;  // Set write bit
    HAL_SPI_Transmit(camera->spi_handler, &addr, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(camera->spi_handler, &data, 1, HAL_MAX_DELAY);
    HAL_Delay(10);
    ov2640_spi_deselect(camera);
}

// Reads a specified byte of data from the OV2640 FIFO buffer through SPI
// Requested byte is written to the address of p_rx_data
void ov2640_fifo_read(ov2640 *camera, uint8_t addr, uint8_t *p_rx_data) {
    ov2640_spi_select(camera);
    HAL_Delay(10);
    addr &= 0x7F;  // Clear write bit
    HAL_SPI_Transmit(camera->spi_handler, &addr, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(camera->spi_handler, p_rx_data, 1, HAL_MAX_DELAY);
    HAL_Delay(10);
    ov2640_spi_deselect(camera);
}

// Clear all data from the OV2640 FIFO buffer 
void ov2640_fifo_clear(ov2640 *camera) {
    ov2640_fifo_write(camera, OV2640_FIFO_CONTROL, OV2640_FIFO_CLEAR_MASK);
    camera->fifo_length = 0;
}

// Perform an OV2640 capture and store it in the FIFO buffer
void ov2640_fifo_start(ov2640 *camera) {
    ov2640_fifo_write(camera, OV2640_FIFO_CONTROL, OV2640_FIFO_START_MASK);
}

// Reads a byte from an address in FIFO containing the desired bit and checks for its status using a mask
// Return value is effectively used as a boolean here
uint8_t ov2640_fifo_check_bit(ov2640 *camera, uint8_t addr, uint8_t mask) {
    uint8_t temp;
    ov2640_fifo_read(camera, addr, &temp);
    return (temp & mask);
}

// Reads the current length of the FIFO buffer and stores it in the ov2640 struct.
void ov2640_fifo_read_length(ov2640 *camera) {
    // The length of the FIFO buffer is stored as three bytes in the OV2640; need to put them together.
    uint8_t len1, len2, len3 = 0;
    ov2640_fifo_read(camera, OV2640_FIFO_SIZE1, &len1);
    ov2640_fifo_read(camera, OV2640_FIFO_SIZE2, &len2);
    ov2640_fifo_read(camera, OV2640_FIFO_SIZE3, &len3);
    len3 = len3 & 0x7F;  // Ensure the highest bit is not considered.

    // Combine the three bytes to obtain the FIFO length.
    camera->fifo_length = ((len3 << 16) | (len2 << 8) | len1) & 0x07FFFFF;
}

// Writes a specified byte of data to a register of the OV2640 sensor through I2C.
void ov2640_sensor_write_byte(ov2640 * camera, uint8_t reg, uint8_t data)
{
	uint8_t buffer[2] = {(reg & 0x00FF), (data & 0x00FF)};

	HAL_I2C_Master_Transmit(camera->i2c_handler, (OV2640_SENSOR_ADDR), buffer, 2, HAL_MAX_DELAY);
}

// Writes a set of byte data to registers of the OV2640 sensor through I2C.
// Should pass in reglists defined in ov2640_regs.h.
void ov2640_sensor_write_bytes(ov2640 *camera, const struct sensor_reg reglist[]) {
    uint16_t reg_addr = 0;
    uint16_t reg_val = 0;
    const struct sensor_reg *next = reglist;

    // Iterate through the list to easily get the next address-value pairing to write.
    // This is safe because the reglists in ov2640_regs.h are explicitly defined beforehand; they always end at {0xff, 0xff}.
    while ((reg_addr != 0xff) || (reg_val != 0xff)) {
        reg_addr = next->reg;
        reg_val = next->val;

        ov2640_sensor_write_byte(camera, reg_addr, reg_val);
        next++;
    }
}

// Reads a specified byte of data from the OV2640 sensor through I2C.
// Requested byte is written to the address of p_rx_data .
void ov2640_sensor_read_byte(ov2640 * camera, uint8_t reg, uint8_t * p_rx_data)
{
	HAL_I2C_Master_Transmit(camera->i2c_handler, OV2640_SENSOR_ADDR, &reg, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(camera->i2c_handler, OV2640_SENSOR_ADDR, p_rx_data, 1, HAL_MAX_DELAY);
}

// Initialize the OV2640 to take captures as JPEG images with a default resolution of 320x240.
void ov2640_jpeg_init(ov2640 * camera)
{
	// Should explicitly start with deslected camera.
	ov2640_spi_deselect(camera);

	// Reset CPLD so configuration starts from a known state.
	ov2640_fifo_write(camera, OV2640_CPLD_ADDR, OV2640_CPLD_REG);
	HAL_Delay(100);
	ov2640_fifo_write(camera, OV2640_CPLD_ADDR, OV2640_CPLD_RESET);
	HAL_Delay(100);

	// Software reset, resets all registers to default values.
	ov2640_sensor_write_byte(camera, 0xff, 0x01);
	ov2640_sensor_write_byte(camera, 0x12, 0x80);
	HAL_Delay(100);

	// Set JPEG format for OV2640.
	ov2640_sensor_write_bytes(camera, OV2640_JPEG_INIT);
	ov2640_sensor_write_bytes(camera, OV2640_YUV422);
	ov2640_sensor_write_bytes(camera, OV2640_JPEG);
	HAL_Delay(100);

	// Set registers to get overwritten without resetting software.
	ov2640_sensor_write_byte(camera, 0xff, 0x01);
	ov2640_sensor_write_byte(camera, 0x15, 0x00);
	HAL_Delay(100);

	// Set default JPEG resolution (320x240) for OV2640.
	ov2640_sensor_write_bytes(camera, OV2640_320x240_JPEG);

	HAL_Delay(1000);

	// FIFO should be empty before making the first capture, so clear it pre-emptively.
	ov2640_fifo_clear(camera);

	// Keep track of the type and resolution of image being captured for future reference.
	camera->image_type = OV2640_IMG_JPEG;
	camera->image_res = OV2640_RES_320x240;
}

// Set the resolution of OV2640 JPEG image captures
void ov2640_jpeg_set_res(ov2640* camera, ov2640_image_res_t image_res)
{
	switch (image_res)
	{
		case OV2640_RES_160x120:
			ov2640_sensor_write_bytes(camera, OV2640_160x120_JPEG);
			break;
		case OV2640_RES_176x144:
			ov2640_sensor_write_bytes(camera, OV2640_176x144_JPEG);
			break;
		case OV2640_RES_352x288:
			ov2640_sensor_write_bytes(camera, OV2640_352x288_JPEG);
			break;
		case OV2640_RES_640x480:
			ov2640_sensor_write_bytes(camera, OV2640_640x480_JPEG);
			break;
		case OV2640_RES_800x600:
			ov2640_sensor_write_bytes(camera, OV2640_800x600_JPEG);
			break;
		case OV2640_RES_1024x768:
			ov2640_sensor_write_bytes(camera, OV2640_1024x768_JPEG);
			break;
		case OV2640_RES_1280x1024:
			ov2640_sensor_write_bytes(camera, OV2640_1280x1024_JPEG);
			break;
		case OV2640_RES_1600x1200:
			ov2640_sensor_write_bytes(camera, OV2640_1600x1200_JPEG);
			break;
		default:
			ov2640_sensor_write_bytes(camera, OV2640_320x240_JPEG);
			break;
	}

	// Keep track of the resolution of image being captured for future reference.
	camera->image_res = image_res;
}

// Take a capture using the OV2640.
// If the capture is obviously invalid, discard it (indicated by the length being reset to 0).
void ov2640_get_capture(ov2640 *camera) {
    // Load the capture into a cleared FIFO buffer.
    ov2640_fifo_clear(camera);
    ov2640_fifo_start(camera);

    // Minimum settling time for capture to load.
    HAL_Delay(100);

    // We can't wait indefinitely for the capture to settle, so a maximum timeout is necessary; set to 1 second for simplicity.
    // If we time out, fifo_length will stay at 0, resulting in discarding the capture.
    for (uint8_t i = 0; i < 10; ++i) {
        if (ov2640_fifo_check_bit(camera, OV2640_CAPTURE_TRIGGER, OV2640_CAPTURE_DONE_MASK)) {
            ov2640_fifo_read_length(camera);
            break;
        }
        HAL_Delay(100);
    }

    // Read and save the length of the capture in the FIFO buffer.
    ov2640_fifo_read_length(camera);

    // Discard a capture by clearing the FIFO buffer if it is obviously invalid based on FIFO length.
    if ((camera->fifo_length > OV2640_CAPTURE_MAX_LENGTH) || (camera->fifo_length < OV2640_CAPTURE_MIN_LENGTH)) {
        ov2640_fifo_clear(camera);
    }
}

// Set the OV2640 to enable reading the capture data in the FIFO buffer to be transferred out.
// Call ov2640_transfer_step to transfer the data out to pre-defined buffers, and call ov2640_transfer_stop when done.
void ov2640_transfer_start(ov2640 * camera)
{
	// SPI must stay selected for the entire duration of the burst read.
	ov2640_spi_select(camera);

	uint8_t fifo_burst_read = OV2640_FIFO_BURST_READ;
	HAL_SPI_Transmit(camera->spi_handler, &fifo_burst_read, 1, HAL_MAX_DELAY);
}

// Copies data from the SPI FIFO buffer into a user buffer.
// buffer_filled is the number of elements in the buffer that actually belong to the image; user buffer is not guaranteed to be 100% filled.
void ov2640_transfer_step(ov2640 * camera, uint8_t buffer[], uint16_t buffer_size, uint16_t *buffer_filled) 
{
	// Determine whether the buffer can get 100% filled and update buffer_filled accordingly
	if(camera->fifo_length > buffer_size) {
		*buffer_filled = buffer_size;
	}
	else {
		*buffer_filled = camera->fifo_length;
	}

	// Receive the maximum amount of data from the SPI FIFO buffer into the user buffer.
	// If the receive succeeds, update the length of the fifo buffer and move on.
	if(HAL_SPI_Receive(camera->spi_handler, buffer, *buffer_filled, HAL_MAX_DELAY) == HAL_OK) {
		camera->fifo_length -= *buffer_filled;
	}
	// If the receive fails, throw out the capture data
	else {
		ov2640_fifo_clear(camera);
	}	
}

// Copies data from the SPI FIFO buffer into a user buffer.
// buffer_filled is the number of elements in the buffer that actually belong to the image; user buffer is not guaranteed to be 100% filled.
// This function uses DMA so the transferring can happen asynchronously. Subtract buffer_filled from camera->fifo_length externally once DMA finishes.
void ov2640_transfer_step_dma(ov2640 * camera, uint8_t buffer[], uint16_t buffer_size, uint16_t *buffer_filled)
{
	// Determine whether the buffer can get 100% filled and update buffer_filled accordingly
	if(camera->fifo_length > buffer_size) {
		*buffer_filled = buffer_size;
	}
	else {
		*buffer_filled = camera->fifo_length;
	}

	// Receive the maximum amount of data from the SPI FIFO buffer into the user buffer.
	HAL_SPI_Receive_DMA(camera->spi_handler, buffer, *buffer_filled);

	// camera->fifo_length should be updated externally when the transfer is complete. Use buffer_filled to do so.
}

// Perform cleanup after data from FIFO buffer is transferred.
void ov2640_transfer_stop(ov2640 * camera)
{
	// SPI must stay selected from calling ov2640_transfer_start up until this point.
	ov2640_spi_deselect(camera);
	ov2640_fifo_clear(camera);
}

// Test I2C by writing a value to a register and reading it back.
uint8_t ov2640_test_i2c(ov2640* camera) {
    uint8_t i2c_initial;
    uint8_t i2c_test;

    // Store the initial value at 0x3b; should be changed back after.
    ov2640_sensor_read_byte(camera, 0x3b, &i2c_initial);

    // If 0x3b's value is 32 after writing, we've proven that I2C works correctly.
    ov2640_sensor_write_byte(camera, 0x3b, 32);
    ov2640_sensor_read_byte(camera, 0x3b, &i2c_test);

    // Restore the initial value at 0x3b.
    ov2640_sensor_write_byte(camera, 0x3b, i2c_initial);

    return (i2c_test == 32);
}

// Test SPI by writing a value to a register and reading it back.
uint8_t ov2640_test_spi(ov2640* camera) {
    uint8_t spi_initial;
    uint8_t spi_test;

    // Store the initial value at 0x00; should be changed back after.
    ov2640_fifo_read(camera, 0x00, &spi_initial);

    // If the value at 0x00 is 0x55 after writing, we've proven that SPI works correctly.
    ov2640_fifo_write(camera, 0x00, 0x55);
    ov2640_fifo_read(camera, 0x00, &spi_test);

    // Restore the initial value at 0x00.
    ov2640_fifo_write(camera, 0x00, spi_initial);

    return (spi_test == 0x55);
}

// Read VID and PID values in OV2640 to verify that this is the correct sensor.
// These values are stored in the ov2640 struct.
void ov2640_test_who_am_i(ov2640* camera) {
    // Get the value of VID; should be 0x26 (38 in decimal).
    ov2640_sensor_write_byte(camera, 0xff, 0x01);
    ov2640_sensor_read_byte(camera, OV2640_CHIPID_HIGH, &(camera->vid));

    // Get the value of PID; should be 0x41 or 0x42 (65 or 66 in decimal).
    ov2640_sensor_write_byte(camera, 0xff, 0x01);
    ov2640_sensor_read_byte(camera, OV2640_CHIPID_LOW, &(camera->pid));
}
