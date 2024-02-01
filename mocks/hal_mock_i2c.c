// hal_mock_i2c.c
#include "hal_mock_i2c.h"

#include <string.h>

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

// Helper function to reset I2C mock instance
static void reset_i2c_instance(I2C_TypeDef *i2cInstance) {
    i2cInstance->DR = 0;
    i2cInstance->SR = 0;
    i2cInstance->OAR = 0;
    i2cInstance->ErrorFlags = HAL_I2C_ERROR_NONE;
    i2cInstance->TransactionInfo.TargetAddress = 0;
    i2cInstance->TransactionInfo.ReadWrite = 0;
    i2cInstance->TransactionInfo.DataSize = 0;
}

// Helper function to clear I2C xfer buffer
static void clear_i2c_buffer(I2C_HandleTypeDef *hi2c) {
    memset(hi2c->pBuff, 0, hi2c->XferSize);
    hi2c->XferSize = 0;
    hi2c->XferCount = 0;
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

      // Clear xfer buffer
      clear_i2c_buffer(hi2c);
    }
    // Unlock the handle
    hi2c->Lock = HAL_UNLOCKED;

    return HAL_OK;
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

        // Clear xfer buffer
        clear_i2c_buffer(hi2c);
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

        // Clear xfer buffer
        clear_i2c_buffer(hi2c);
    }
    // Unlock the I2C handle when done using it
    hi2c->Lock = HAL_UNLOCKED;

    return HAL_OK;
}
