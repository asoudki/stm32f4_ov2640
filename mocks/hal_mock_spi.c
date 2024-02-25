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
    hspi->ErrorCode = HAL_SPI_ERROR_HAL_UNINITIALIZED;
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

  // Catch SPI in an invalid state
  if(hspi->State == HAL_SPI_STATE_RESET) {
    hspi->ErrorCode = HAL_SPI_ERROR_UNINITIALIZED;
    return HAL_ERROR;
  }
  else if(hspi->State == HAL_SPI_STATE_ERROR) {
    hspi->ErrorCode = HAL_SPI_ERROR_FAILSTATE;
    return HAL_ERROR;
  }

  // Check if the size of the message is too big for the mock msg buffer
  if(Size >= MOCK_SPI_MAX_MSG_SIZE) {
    hspi->ErrorCode = HAL_SPI_ERROR_MSG_TOO_BIG;
    return HAL_ERROR;
  }

  return HAL_OK; // No error
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

    // Clear data relating to any previous messages
    memset(hspi->TxMsgBuff, 0, sizeof(hspi->TxMsgBuff));
    hspi->TxMsgSize = 0;
    memset(hspi->RxMsgBuff, 0, sizeof(hspi->RxMsgBuff));
    hspi->RxMsgSize = 0;

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

    // Clear data relating to any previous messages
    memset(hspi->TxMsgBuff, 0, sizeof(hspi->TxMsgBuff));
    hspi->TxMsgSize = 0;
    memset(hspi->RxMsgBuff, 0, sizeof(hspi->RxMsgBuff));
    hspi->RxMsgSize = 0;

    // Set SPI to a reset state; cannot perform transactions now
    hspi->State = HAL_SPI_STATE_RESET;
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;

    return HAL_OK;
}

// Transmit an amount of data in blocking mode
// Call Mock_SPI_Slave_Receive on slave end after calling this function to finish transaction
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

    // Wait for any ongoing SPI transactions to finish before transmitting data
    // Time out if the waiting process takes too long
    while(hspi->State != HAL_SPI_STATE_READY) {
        if(Timeout == 0) {
            hspi->ErrorCode = HAL_SPI_ERROR_TIMEOUT;
            return HAL_ERROR;
        }
        usleep(1000);
        Timeout--;
    }

    // Simulate transaction by copying data to the message buffer, access using TxMsgBuff and TxMsgSize
    memcpy(hspi->TxMsgBuff, pData, Size);
    // Record size of the message to be used in slave receive
    hspi->TxMsgSize = Size;
    // Change state to indicate that a master transmit has started
    hspi->State = HAL_SPI_STATE_BUSY_TX;
    // Clear error code to indicate successful initiation of transfer
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;

    return HAL_OK;
}

// Transmit an amount of data in DMA mode
// Is the same as HAL_SPI_Transmit for the purpose of our mock
HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size) {
    return HAL_SPI_Transmit(hspi, pData, Size, HAL_MAX_DELAY);
}

// Receive an amount of data in blocking mode
// Call Mock_SPI_Slave_Transmit on slave end after calling this function to finish transaction
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

    // Indicate through RxMsgSize the amount of data requested
    hspi->RxMsgSize = Size;

    // Wait for slave to send data to SPI before receiving data
    // Time out if the waiting process takes too long
    while(hspi->State != HAL_SPI_STATE_BUSY_RX) {
        if(Timeout == 0) {
            hspi->ErrorCode = HAL_SPI_ERROR_TIMEOUT;
            return HAL_ERROR;
        }
        usleep(1000);
        Timeout--;
    }

    // Simulate transaction by copying data from the message buffer, access using pData and Size
    memcpy(pData, hspi->RxMsgBuff, Size);
    // Change state to indicate that the data sent from slave has been received
    hspi->State = HAL_SPI_STATE_READY;
    // Clear error code to indicate successful transfer
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;
    // Clear RxMsgSize to indicate that the requested data has been received
    hspi->RxMsgSize = 0;

    return HAL_OK;
}

// Receive an amount of data in DMA mode
// Is the same as HAL_SPI_Receive for the purpose of our mock
HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size) {
    return HAL_SPI_Receive(hspi, pData, Size, HAL_MAX_DELAY);
}

// Transmit data from mock slave device for master to receive
// Is called before HAL_SPI_Receive or HAL_SPI_Receive_DMA
HAL_StatusTypeDef Mock_SPI_Slave_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
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

    // Wait for any ongoing SPI transactions to finish before transmitting data
    // Time out if the waiting process takes too long
    while(hspi->State != HAL_SPI_STATE_READY) {
        if(Timeout == 0) {
            hspi->ErrorCode = HAL_SPI_ERROR_TIMEOUT;
            return HAL_ERROR;
        }
        usleep(1000);
        Timeout--;
    }

    // Check that the size of the message to be sent is the one requested by the master
    if(Size != hspi->RxMsgSize) {
        hspi->ErrorCode = HAL_SPI_ERROR_SIZE_MISMATCH;
        return HAL_ERROR;
    }

    // Simulate transaction by copying data to the message buffer, access using RxMsgBuff and RxMsgSize
    memcpy(hspi->RxMsgBuff, pData, Size);
    // Change state to indicate that a master receive is expected
    hspi->State = HAL_SPI_STATE_BUSY_RX;
    // Clear error code to indicate successful transfer
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;

    return HAL_OK;
}

// Receive data on mock slave device from master transmit
// Is called after HAL_SPI_Transmit or HAL_SPI_Transmit_DMA
HAL_StatusTypeDef Mock_SPI_Slave_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
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

    // Wait for master to send data to SPI before receiving data
    // Time out if the waiting process takes too long
    while(hspi->State != HAL_SPI_STATE_BUSY_TX) {
        if(Timeout == 0) {
            hspi->ErrorCode = HAL_SPI_ERROR_TIMEOUT;
            return HAL_ERROR;
        }
        usleep(1000);
        Timeout--;
    }

    // Check that the size of the message to be received is the one sent by the master
    if(Size != hspi->TxMsgSize) {
        hspi->ErrorCode = HAL_SPI_ERROR_SIZE_MISMATCH;
        return HAL_ERROR;
    }

    // Simulate transaction by copying data from the message buffer, access using pData and Size
    memcpy(pData, hspi->TxMsgBuff, Size);
    // Change state to indicate that the data sent from master has been received
    hspi->State = HAL_SPI_STATE_READY;
    // Clear error code to indicate successful transfer
    hspi->ErrorCode = HAL_SPI_ERROR_NONE;

    return HAL_OK;
}