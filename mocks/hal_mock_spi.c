// hal_mock_spi.c
#include "hal_mock_spi.h"
#include <string.h>

// Check for common errors in the SPI HAL before proceeding with a SPI HAL function
static HAL_StatusTypeDef common_spi_checks(SPI_HandleTypeDef *hspi) {
  // Catch invalid SPI handle
  if(hspi == NULL) {
    return HAL_ERROR;
  }

  // Catch uninitialized HAL
  if(!hal_initialized) {
    hspi->ErrorCode = HAL_SPI_ERROR_HAL_UNITIALIZED;
    return HAL_ERROR;
  }

  return HAL_OK; // No error
}

// Check for common errors in the SPI HAL before proceeding with a SPI HAL function that involves a transaction
static HAL_StatusTypeDef common_spi_transaction_checks(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size) {
  // Catch null pData
  if(pData == NULL) {
    hspi->ErrorCode = HAL_SPI_ERROR_NULL_PARAM;
    return HAL_ERROR;
  }

  // Catch SPI in a non-ready state
  if(hspi->State == HAL_SPI_STATE_RESET) {
    hspi->ErrorCode = HAL_SPI_ERROR_UNITIALIZED;
    return HAL_ERROR;
  }
  else if(hspi->State == HAL_SPI_STATE_BUSY) {
    hspi->ErrorCode = HAL_SPI_ERROR_BUSY;
    return HAL_BUSY;
  }
  else if(hspi->State == HAL_SPI_STATE_ERROR) {
    hspi->ErrorCode = HAL_SPI_ERROR_FAILSTATE;
    return HAL_ERROR;
  }

  return HAL_OK; // No error
}

// Set a mock buffer as a slave; HAL_SPI_Transmit or HAL_SPI_Transmit_DMA will copy to this buffer
// This function is used in the absence of setting the CS pin to avoid cross-dependencies with GPIO in mock
HAL_StatusTypeDef Set_SPI_Transmit_Mock_Slave(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size) {
  // Check for common errors
  HAL_StatusTypeDef status = common_spi_checks(hspi);
  if(status != HAL_OK) {
    return status;
  }

  // Set the SPI handle to the mock slave buffer
  hspi->pTxBuffPtr = pData;
  hspi->TxXferSize = Size;

  return HAL_OK;
}

// Set a mock buffer as a slave; HAL_SPI_Receive or HAL_SPI_Receive_DMA will copy from this buffer
// This function is used in the absence of setting the CS pin to avoid cross-dependencies with GPIO in mock
HAL_StatusTypeDef Set_SPI_Receive_Mock_Slave(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size) {
  // Check for common errors
  HAL_StatusTypeDef status = common_spi_checks(hspi);
  if(status != HAL_OK) {
    return status;
  }

  // Set the SPI handle to the mock slave buffer
  hspi->pRxBuffPtr = pData;
  hspi->RxXferSize = Size;

  return HAL_OK;
}


// Initializes the SPI peripheral
// Assumes a default configuration (master, 8-bit data size, 2 lines, low polarity, 1 edge phase, software NSS, MSB first, TI mode disabled, and CRC calculation disabled) to focus on logic
// Baudrate is omitted for this implementation for similar reasons
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi) {
    // Check for common errors
    HAL_StatusTypeDef status = common_spi_checks(hspi);
    if (status != HAL_OK) {
        return status;
    }

    // Set SPI to a ready state; can perform transactions now
    hspi->State = HAL_SPI_STATE_READY;
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;

    return HAL_OK;
}

// Deinitializes the SPI peripheral
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi) {
    // Check for common errors
    HAL_StatusTypeDef status = common_spi_checks(hspi);
    if (status != HAL_OK) {
        return status;
    }

    // Set SPI to a reset state; cannot perform transactions now
    hspi->State = HAL_SPI_STATE_RESET;
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;

    return HAL_OK;
}

// Transmit an amount of data in blocking mode
// Call Set_SPI_Transmit_Mock_Slave first to set the mock slave buffer receiving the data
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    // Check for common errors
    HAL_StatusTypeDef status = common_spi_checks(hspi);
    if (status != HAL_OK) {
        return status;
    }

    // Check for common transaction errors
    HAL_StatusTypeDef transaction_status = common_spi_transaction_checks(hspi, pData, Size);
    if (transaction_status != HAL_OK) {
        return transaction_status;
    }

    // Check that the mock slave set by Set_SPI_Transmit_Mock_Slave is a valid buffer of the same size as input data
    if(hspi->pTxBuffPtr == NULL || hspi->TxXferSize != Size) {
      hspi->ErrorCode = HAL_SPI_ERROR_BAD_SLAVE;
      return HAL_ERROR;
    }

    // Copy input data to slave buffer to simulate transfer
    memcpy(hspi->pTxBuffPtr, pData, Size);
    // Clear error code to indicate successful transfer
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;

    return HAL_OK;
}

// Transmit an amount of data in DMA mode
// Is the same as HAL_SPI_Transmit when mocked (Can assume DMA is instantenous in this environment)
HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size) {
    return HAL_SPI_Transmit(hspi, pData, Size, 0);
}

// Receive an amount of data in blocking mode
// Call Set_SPI_Receive_Mock_Slave first to set the mock slave buffer sending the data
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    // Check for common errors
    HAL_StatusTypeDef status = common_spi_checks(hspi);
    if (status != HAL_OK) {
        return status;
    }

    // Check for common transaction errors
    HAL_StatusTypeDef transaction_status = common_spi_transaction_checks(hspi, pData, Size);
    if (transaction_status != HAL_OK) {
        return transaction_status;
    }

    // Check that the mock slave set by Set_SPI_Receive_Mock_Slave is a valid buffer of the same size as input data
    if(hspi->pRxBuffPtr == NULL || hspi->RxXferSize != Size) {
      hspi->ErrorCode = HAL_SPI_ERROR_BAD_SLAVE;
      return HAL_ERROR;
    }

    // Copy input data to slave buffer to simulate transfer
    memcpy(pData, hspi->pRxBuffPtr, Size);
    // Clear error code to indicate successful transfer
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;

    return HAL_OK;
}

// Receive an amount of data in DMA mode
// Is the same as HAL_SPI_Receive when mocked (Can assume DMA is instantenous in this environment)
HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size) {
    return HAL_SPI_Receive(hspi, pData, Size, 0);
}