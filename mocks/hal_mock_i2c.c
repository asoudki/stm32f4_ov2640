// hal_mock_i2c.c
#include "hal_mock_i2c.h"

static struct {
  uint8_t *txBuffer;
  uint16_t txBufferSize;
  HAL_StatusTypeDef result;
} i2cMockData;

void configureI2CMock(uint8_t *txBuffer, uint16_t txBufferSize, HAL_StatusTypeDef result) {
  i2cMockData.txBuffer = txBuffer;
  i2cMockData.txBufferSize = txBufferSize;
  i2cMockData.result = result;
}

HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
  // Mock implementation for HAL_I2C_Master_Transmit
  // Simulate I2C transmission based on the configured mock data
  // Additional logic for handling different scenarios can be added here
  return i2cMockData.result;
}
