// hal_mock_i2c.c
#include "hal_mock_i2c.h"

#include <string.h>

// I2C slave and data for current transaction
I2C_Mock_Slave I2C_Mock_Slave_Instance;

// Global error code used to inject errors into I2C HAL transactions
uint32_t I2C_Injected_Error = HAL_I2C_ERROR_NONE;

// Helper function to check if the provided I2C handle is good to init/deinit
static HAL_StatusTypeDef common_i2c_checks(I2C_HandleTypeDef *hi2c) {
    // Catch invalid I2C handle
    if (hi2c == NULL) {
        return HAL_ERROR;
    }
    
    // Catch uninitialized HAL
    if (!hal_initialized) {
        hi2c->ErrorCode = HAL_I2C_ERROR_HAL_UNITIALIZED;
        return HAL_ERROR;
    }

    // Catch invalid I2C instance
    if (hi2c->Instance == NULL) {
        hi2c->ErrorCode = HAL_I2C_ERROR_INVALID_INSTANCE;
        return HAL_ERROR;
    }

    // Check if the I2C handle is already in use
    if (hi2c->Lock == HAL_LOCKED) {
        hi2c->ErrorCode = HAL_I2C_ERROR_BUSY;
        return HAL_BUSY;
    }

    return HAL_OK; // No error
}

static HAL_StatusTypeDef common_i2c_master_transaction_checks(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
    // Check for null pData
    if(pData == NULL) {
        hi2c->ErrorCode = HAL_I2C_ERROR_NULL_PARAM;
        return HAL_ERROR;
    }

    // Check for a mismatch in mocked slave address
    if((hi2c->Instance->OAR != DevAddress) || (I2C_Mock_Slave_Instance.Address != DevAddress)) {
        hi2c->ErrorCode = HAL_I2C_ERROR_AF;
        return HAL_ERROR;
    }
    
    // Ensure the peripheral is not busy before transmitting or receiving
    if(hi2c->State != HAL_I2C_STATE_READY) {
        hi2c->ErrorCode = HAL_I2C_ERROR_BUSY;
        return HAL_BUSY;
    }

    // Check if an injected error should otherwise be returned
    if(I2C_Injected_Error != HAL_I2C_ERROR_NONE) {
        hi2c->ErrorCode = I2C_Injected_Error;
        return HAL_ERROR;
    }
}

// Helper function to reset I2C mock instance
static void reset_i2c_instance(I2C_TypeDef *i2cInstance) {
    i2cInstance->DR = 0;
    i2cInstance->OAR = 0;
}

// Reset the I2C handle to a default state
HAL_StatusTypeDef Reset_I2C_Handle(I2C_HandleTypeDef *hi2c) {
    // Perform common checks for I2C handle reset
    HAL_StatusTypeDef status = common_i2c_checks(hi2c);
    if (status != HAL_OK) {
        return status;
    }

    // Lock the I2C handle while in use
    hi2c->Lock = HAL_LOCKED;
    {
      // Reset the I2C mock instance
      reset_i2c_instance(hi2c->Instance);

      // Reset handle state, mode and error code
      hi2c->State = HAL_I2C_STATE_RESET;
      hi2c->Mode = HAL_I2C_MODE_NONE;
      hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    }
    // Unlock the handle
    hi2c->Lock = HAL_UNLOCKED;

    return HAL_OK;
}

// Reset the I2C slave device for the current master transaction
void Reset_I2C_Mock_Slave(void) {
    I2C_Mock_Slave_Instance.Address = 0;
    I2C_Mock_Slave_Instance.Buffer = NULL;
    I2C_Mock_Slave_Instance.Size = 0;
}

// Get a copy of the I2C slave device for the current master transaction
I2C_Mock_Slave Get_I2C_Mock_Slave(void) {
    return I2C_Mock_Slave_Instance;
}

// Set the I2C slave device for the current master transaction
// Should always be called before a master transaction; can create a dangling pointer if pointer for current buffer falls out of scope
HAL_StatusTypeDef Set_I2C_Mock_Slave(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
    // Check for null args
    if(hi2c == NULL || pData == NULL) {
        return HAL_ERROR;
    }
    
    // Check for invalid size
    if(Size == 0 || Size > HAL_I2C_MOCK_BUFFER_SIZE) {
        hi2c->ErrorCode = HAL_I2C_ERROR_SIZE;
        return HAL_ERROR;
    }

    hi2c->Instance->OAR = DevAddress;
    I2C_Mock_Slave_Instance.Address = DevAddress;
    I2C_Mock_Slave_Instance.Buffer = pData;
    I2C_Mock_Slave_Instance.Size = Size;

    return HAL_OK;
}

// Inject an error (including HAL_I2C_ERROR_NONE to clear error) into I2C transactions
void Inject_I2C_Transaction_Error(uint32_t ErrorCode) {
    I2C_Injected_Error = ErrorCode;
}

// Initializes the I2C peripheral
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c) {
    // Perform common checks for I2C handle initialization
    HAL_StatusTypeDef status = common_i2c_checks(hi2c);
    if (status != HAL_OK) {
        return status;
    }

    // Ensure the peripheral is not already initialized before initializing
    if (hi2c->State != HAL_I2C_STATE_RESET && hi2c->State != HAL_I2C_STATE_ERROR) {
        hi2c->ErrorCode = HAL_I2C_ERROR_ALREADY_INITIALIZED;
        return HAL_ERROR;
    }

    // Check I2C instance parameters if they need to be used

    // Lock the I2C handle while in use
    hi2c->Lock = HAL_LOCKED;
    {
        // Reset the I2C mock instance
        reset_i2c_instance(hi2c->Instance);

        // Set state, mode and error code
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    }
    // Unlock the I2C handle when done using it
    hi2c->Lock = HAL_UNLOCKED;

    return HAL_OK;
}

// Deinitializes the I2C peripheral
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c) {
    // Perform common checks for I2C handle deinitialization
    HAL_StatusTypeDef status = common_i2c_checks(hi2c);
    if (status != HAL_OK) {
        return status;
    }

    // Ensure the peripheral is not busy before deinitializing
    if (hi2c->State != HAL_I2C_STATE_RESET && hi2c->State != HAL_I2C_STATE_READY) {
        hi2c->ErrorCode = HAL_I2C_ERROR_BUSY;
        return HAL_BUSY;
    }

    // Lock the I2C handle while in use
    hi2c->Lock = HAL_LOCKED;
    {
        // Reset the I2C mock instance
        reset_i2c_instance(hi2c->Instance);

        // Reset state, mode, and error code
        hi2c->State = HAL_I2C_STATE_RESET;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    }
    // Unlock the I2C handle when done using it
    hi2c->Lock = HAL_UNLOCKED;

    return HAL_OK;
}

// Transmit in master mode an amount of data in blocking mode
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    // Perform common checks for I2C handle master transmit
    HAL_StatusTypeDef status = common_i2c_checks(hi2c);
    if (status != HAL_OK) {
        return status;
    }

    // Perform master transaction checks for I2C handle master transmit
    status = common_i2c_master_transaction_checks(hi2c, DevAddress, pData, Size);
    if (status != HAL_OK) {
        return status;
    }

    // Lock the I2C handle while in use
    hi2c->Lock = HAL_LOCKED;
    {
        // Set the I2C instance to busy
        hi2c->State = HAL_I2C_STATE_BUSY_TX;

        // Set the I2C instance to master mode
        hi2c->Mode = HAL_I2C_MODE_MASTER;

        // Copy input data to slave buffer to simulate transfer
        hi2c->pBuff = pData;
        hi2c->XferSize = Size;
        memcpy(I2C_Mock_Slave_Instance.Buffer, hi2c->pBuff, hi2c->XferSize);
        hi2c->XferCount = hi2c->XferSize;
        hi2c->Instance->DR = hi2c->pBuff[Size-1];

        // Reset state, mode and error code
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    }
    // Unlock the I2C handle when done using it
    hi2c->Lock = HAL_UNLOCKED;

    return HAL_OK;
}

// Receive in master mode an amount of data in blocking mode
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    // Perform common checks for I2C handle master receive
    HAL_StatusTypeDef status = common_i2c_checks(hi2c);
    if (status != HAL_OK) {
        return status;
    }

    // Perform master transaction checks for I2C handle master receive
    status = common_i2c_master_transaction_checks(hi2c, DevAddress, pData, Size);
    if (status != HAL_OK) {
        return status;
    }

    // Lock the I2C handle while in use
    hi2c->Lock = HAL_LOCKED;
    {
        // Set the I2C instance to busy
        hi2c->State = HAL_I2C_STATE_BUSY_RX;

        // Set the I2C instance to master mode
        hi2c->Mode = HAL_I2C_MODE_MASTER;

        // Copy slave buffer to input buffer to simulate transfer
        hi2c->pBuff = pData;
        hi2c->XferSize = Size;
        memcpy(hi2c->pBuff, I2C_Mock_Slave_Instance.Buffer, hi2c->XferSize);
        hi2c->XferCount = hi2c->XferSize;
        hi2c->Instance->DR = I2C_Mock_Slave_Instance.Buffer[Size-1];

        // Reset state, mode and error code
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    }
    // Unlock the I2C handle when done using it
    hi2c->Lock = HAL_UNLOCKED;

    return HAL_OK;
}