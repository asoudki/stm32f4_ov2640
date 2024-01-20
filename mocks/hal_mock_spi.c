// hal_mock_spi.c
#include "hal_mock_spi.h"

void HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size) {
  // Mock implementation for HAL_SPI_Transmit_DMA
}

HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size) {
  // Mock implementation for HAL_SPI_Receive_DMA
  return HAL_OK;
}

void HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
  // Mock implementation for HAL_SPI_Transmit
}

HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
  // Mock implementation for HAL_SPI_Receive
  return HAL_OK;
}