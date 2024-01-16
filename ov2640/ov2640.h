#ifndef OV2640_H
#define OV2640_H

// sensor_reg definition must come before ov2640_regs.h inclusion, as ov2640_regs.h uses this definition
#include <stdint.h>
// is uint16_t just an error I carried over?
struct sensor_reg {
	uint8_t reg;
	uint8_t val;
};

#include "stm32f4xx_hal.h"
#include "ov2640_regs.h"


#define OV2640_CPLD_ADDR              	0x07
#define OV2640_CPLD_REG               	0x80
#define OV2640_CPLD_RESET             	0x00

#define OV2640_CHIPID_HIGH				0x0A
#define OV2640_CHIPID_LOW				0x0B
#define OV2640_SENSOR_ADDR 				0x60

#define OV2640_FIFO_CONTROL				0x04
#define OV2640_FIFO_CLEAR_MASK			0x01
#define OV2640_FIFO_START_MASK			0x02
#define OV2640_FIFO_BURST_READ			0x3C
#define OV2640_FIFO_SINGLE_READ			0x3D

#define OV2640_FIFO_SIZE1				0x42
#define OV2640_FIFO_SIZE2				0x43
#define OV2640_FIFO_SIZE3				0x44

#define OV2640_CAPTURE_TRIGGER			0x41
#define OV2640_CAPTURE_DONE_MASK		0x08

#define OV2640_CAPTURE_MIN_LENGTH     	1
#define OV2640_CAPTURE_MAX_LENGTH     	0x5FFFE

typedef enum ov2640_image_type
{
	OV2640_IMG_ERR,
	OV2640_IMG_JPEG
} ov2640_image_type_t;

typedef enum ov2640_image_res
{
	OV2640_RES_ERR,
	OV2640_RES_160x120,
	OV2640_RES_176x144,
	OV2640_RES_320x240,
	OV2640_RES_352x288,
	OV2640_RES_640x480,
	OV2640_RES_800x600,
	OV2640_RES_1024x768,
	OV2640_RES_1280x1024,
	OV2640_RES_1600x1200
} ov2640_image_res_t;

typedef struct ov2640 {
	// Handlers and whatnot for STM32 HAL
	GPIO_TypeDef * spi_cs_port;
	uint16_t spi_cs_pin;
	SPI_HandleTypeDef * spi_handler;
	I2C_HandleTypeDef * i2c_handler;

	// Type of image being captured
	ov2640_image_type_t image_type;
	ov2640_image_res_t image_res;

	// Current length of FIFO buffer containing capture data; is non-zero if there is an outstanding capture
	uint32_t fifo_length;

	// IDs regarding the type of camera
	uint8_t vid;
	uint8_t pid;
} ov2640;

// Setup functions
void ov2640_setup(ov2640 *camera, GPIO_TypeDef *spi_cs_port, uint16_t spi_cs_pin, SPI_HandleTypeDef * spi_handler, I2C_HandleTypeDef * i2c_handler);

// FIFO (SPI) functions
void ov2640_spi_select(ov2640 * camera);
void ov2640_spi_deselect(ov2640 * camera);

void ov2640_fifo_write(ov2640 * camera, uint8_t addr, uint8_t data);
void ov2640_fifo_read(ov2640 * camera, uint8_t addr, uint8_t * p_rx_data);
void ov2640_fifo_clear(ov2640 * camera);
void ov2640_fifo_start(ov2640 * camera);

uint8_t ov2640_fifo_check_bit(ov2640 * camera, uint8_t addr, uint8_t mask);
void ov2640_fifo_read_length(ov2640 * camera);

// Sensor configuration (I2C) functions
void ov2640_sensor_write_byte(ov2640 * camera, uint8_t reg, uint8_t data);
void ov2640_sensor_write_bytes(ov2640 * camera, const struct sensor_reg reglist[]);
void ov2640_sensor_read_byte(ov2640 * camera, uint8_t reg, uint8_t * p_rx_data);

// Initialization functions
void ov2640_jpeg_init(ov2640 * camera);
void ov2640_jpeg_set_res(ov2640* camera, ov2640_image_res_t image_res);

// Image capture functions
void ov2640_get_capture(ov2640 * camera);

// Image handling functions
void ov2640_transfer_start(ov2640 * camera);
void ov2640_transfer_step(ov2640 * camera, uint8_t buffer[], uint16_t buffer_size, uint16_t * buffer_filled);
void ov2640_transfer_step_dma(ov2640 * camera, uint8_t buffer[], uint16_t buffer_size, uint16_t *buffer_filled);
void ov2640_transfer_stop(ov2640 * camera);

// Sanity testing functions to be used at runtime
uint8_t ov2640_test_i2c(ov2640* camera);
uint8_t ov2640_test_spi(ov2640* camera);
void ov2640_test_who_am_i(ov2640* camera);

#endif // OV2640_H
