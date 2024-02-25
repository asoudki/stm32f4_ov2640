// hal_mock_i2c.c
#include "hal_mock_i2c.h"

#include <string.h>

// Check for common errors in the I2C HAL before proceeding with a I2C HAL function
static HAL_StatusTypeDef common_i2c_checks(I2C_HandleTypeDef *hi2c) {
    // Catch invalid I2C handle
    if (hi2c == NULL) {
        return HAL_ERROR;
    }
    
    // Catch uninitialized HAL
    if (!hal_initialized) {
        hi2c->ErrorCode = HAL_I2C_ERROR_HAL_UNINITIALIZED;
        return HAL_ERROR;
    }

    return HAL_OK; // No error
}

// Check for common errors in the I2C HAL before proceeding with a I2C HAL function that involves a transaction
static HAL_StatusTypeDef common_i2c_master_transaction_checks(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
    // Check for null pData
    if(pData == NULL) {
        hi2c->ErrorCode = HAL_I2C_ERROR_NULL_PARAM;
        return HAL_ERROR;
    }
    
    // Catch I2C in an invalid state
    if(hi2c->State == HAL_I2C_STATE_RESET) {
        hi2c->ErrorCode = HAL_I2C_ERROR_UNINITIALIZED;
        return HAL_ERROR;
    }
    else if(hi2c->State == HAL_I2C_STATE_ERROR) {
        hi2c->ErrorCode = HAL_I2C_ERROR_FAILSTATE;
        return HAL_ERROR;
    }

    // Check if the size of the message is too big for the mock msg buffer
    if(Size >= MOCK_I2C_MAX_MSG_SIZE) {
        hi2c->ErrorCode = HAL_I2C_ERROR_MSG_TOO_BIG;
        return HAL_ERROR;
    }

    return HAL_OK;
}

// Initializes the I2C peripheral
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c) {
    // Check for common errors
    HAL_StatusTypeDef status = common_i2c_checks(hi2c);
    if (status != HAL_OK) {
        return status;
    }

    // Clear data relating to any previous messages
    hi2c->XferAddress = 0;
    memset(hi2c->MsgBuff, 0, sizeof(hi2c->MsgBuff));
    hi2c->MsgSize = 0;

    // Set I2C to a ready state; can perform transactions now
    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    return HAL_OK;
}

// Deinitializes the I2C peripheral
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c) {
    // Check for common errors
    HAL_StatusTypeDef status = common_i2c_checks(hi2c);
    if (status != HAL_OK) {
        return status;
    }

    // Clear data relating to any previous messages
    hi2c->XferAddress = 0;
    memset(hi2c->MsgBuff, 0, sizeof(hi2c->MsgBuff));
    hi2c->MsgSize = 0;

    // Set I2C to a reset state; cannot perform transactions now
    hi2c->State = HAL_I2C_STATE_RESET;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    return HAL_OK;
}

// Transmit in master mode an amount of data in blocking mode
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    // Check for common errors
    HAL_StatusTypeDef status = common_i2c_checks(hi2c);
    if (status != HAL_OK) {
        return status;
    }

    // Check for common transaction errors
    HAL_StatusTypeDef transaction_status = common_i2c_master_transaction_checks(hi2c, DevAddress, pData, Size);
    if (transaction_status != HAL_OK) {
        return transaction_status;
    }

    // Wait for any ongoing I2C transactions to finish before transmitting data
    // Time out if the waiting process takes too long
    while(hi2c->State != HAL_I2C_STATE_READY) {
        if(Timeout == 0) {
            hi2c->ErrorCode = HAL_I2C_ERROR_TIMEOUT;
            return HAL_ERROR;
        }
        usleep(1000);
        Timeout--;
    }

    // Simulate transaction by copying data to the message buffer, access using MsgBuff and MsgSize
    memcpy(hi2c->MsgBuff, pData, Size);
    // Record address of slave to receive from and size of message
    hi2c->XferAddress = DevAddress;
    hi2c->MsgSize = Size;
    // Change state to indicate that a master transmit has started
    hi2c->State = HAL_I2C_STATE_BUSY_TX;
    // Clear error code to indicate successful transfer
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    return HAL_OK;
}

// Receive in master mode an amount of data in blocking mode
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    // Check for common errors
    HAL_StatusTypeDef status = common_i2c_checks(hi2c);
    if (status != HAL_OK) {
        return status;
    }

    // Check for common transaction errors
    HAL_StatusTypeDef transaction_status = common_i2c_master_transaction_checks(hi2c, DevAddress, pData, Size);
    if (transaction_status != HAL_OK) {
        return transaction_status;
    }

    // Indicate through MsgSize the amount of data requested
    hi2c->MsgSize = Size;

    // Wait for slave to send data to I2C before receiving data
    // Time out if the waiting process takes too long
    while(hi2c->State != HAL_I2C_STATE_BUSY_RX) {
        if(Timeout == 0) {
            hi2c->ErrorCode = HAL_I2C_ERROR_TIMEOUT;
            return HAL_ERROR;
        }
        usleep(1000);
        Timeout--;
    }

    // Simulate transaction by copying data from the message buffer to the input buffer, access using pData and MsgSize
    memcpy(pData, hi2c->MsgBuff, Size);
    // Record data corresponding to the transaction
    hi2c->XferAddress = DevAddress;
    // Change state to indicate that the data sent from slave has been received
    hi2c->State = HAL_I2C_STATE_READY;
    // Clear error code to indicate successful transfer
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    // Clear MsgSize to indicate that the requested data has been received
    hi2c->MsgSize = 0;

    return HAL_OK;
}

// Transmit data from mock slave device for master to receive
// Is called before HAL_I2C_Master_Receive
HAL_StatusTypeDef Mock_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    // Check for common errors
    HAL_StatusTypeDef status = common_i2c_checks(hi2c);
    if (status != HAL_OK) {
        return status;
    }

    // Check for common transaction errors
    HAL_StatusTypeDef transaction_status = common_i2c_master_transaction_checks(hi2c, hi2c->XferAddress, pData, Size);
    if (transaction_status != HAL_OK) {
        return transaction_status;
    }

    // Wait for any ongoing I2C transactions to finish before transmitting data
    // Time out if the waiting process takes too long
    while(hi2c->State != HAL_I2C_STATE_READY) {
        if(Timeout == 0) {
            hi2c->ErrorCode = HAL_I2C_ERROR_TIMEOUT;
            return HAL_ERROR;
        }
        usleep(1000);
        Timeout--;
    }

    // Check that the size of the message to be sent is the one requested by the master
    if(Size != hi2c->MsgSize) {
        hi2c->ErrorCode = HAL_I2C_ERROR_SIZE_MISMATCH;
        return HAL_ERROR;
    }

    // Simulate transaction by copying data to the message buffer, access using MsgBuff and MsgSize
    memcpy(hi2c->MsgBuff, pData, Size);
    // Change state to indicate that a master receive is expected
    hi2c->State = HAL_I2C_STATE_BUSY_TX;
    // Clear error code to indicate successful transfer
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    return HAL_OK;
}

// Receive data on mock slave device from master transmit
// Is called after HAL_I2C_Master_Transmit
HAL_StatusTypeDef Mock_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    // Check for common errors
    HAL_StatusTypeDef status = common_i2c_checks(hi2c);
    if (status != HAL_OK) {
        return status;
    }

    // Check for common transaction errors
    HAL_StatusTypeDef transaction_status = common_i2c_master_transaction_checks(hi2c, hi2c->XferAddress, pData, Size);
    if (transaction_status != HAL_OK) {
        return transaction_status;
    }

    // Wait for master to send data to I2C before receiving data
    // Time out if the waiting process takes too long
    while(hi2c->State != HAL_I2C_STATE_BUSY_TX) {
        if(Timeout == 0) {
            hi2c->ErrorCode = HAL_I2C_ERROR_TIMEOUT;
            return HAL_ERROR;
        }
        usleep(1000);
        Timeout--;
    }

    // Check that the size of the message to be received is the one sent by the master
    if(Size != hi2c->MsgSize) {
        hi2c->ErrorCode = HAL_I2C_ERROR_SIZE_MISMATCH;
        return HAL_ERROR;
    }

    // Simulate transaction by copying data from the message buffer to the input buffer, access using pData and MsgSize
    memcpy(pData, hi2c->MsgBuff, Size);
    // Change state to indicate that the data sent from master has been received
    hi2c->State = HAL_I2C_STATE_READY;
    // Clear error code to indicate successful transfer
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    return HAL_OK;
}