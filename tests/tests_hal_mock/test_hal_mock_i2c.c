#include "test_hal_mock_i2c.h"

// Defiinition of test arrays

// common_i2c_checks Tests
const struct CMUnitTest common_i2c_checks_tests[NUM_COMMON_I2C_CHECKS_TESTS] = {
    cmocka_unit_test(test_common_i2c_checks_returns_ok),
    cmocka_unit_test(test_common_i2c_checks_null_input),
    cmocka_unit_test(test_common_i2c_checks_no_hal_init),
    cmocka_unit_test(test_common_i2c_checks_invalid_instance),
    cmocka_unit_test(test_common_i2c_checks_handle_busy),
};

// Reset_I2C_Handle Tests
const struct CMUnitTest hal_mock_reset_i2c_handle_tests[NUM_HAL_MOCK_RESET_I2C_HANDLE_TESTS] = {
    cmocka_unit_test(test_hal_mock_reset_i2c_handle_changes_state),
    cmocka_unit_test(test_hal_mock_reset_i2c_handle_changes_mode),
    cmocka_unit_test(test_hal_mock_reset_i2c_handle_from_error),
    cmocka_unit_test(test_hal_mock_reset_i2c_handle_left_unlocked),
    cmocka_unit_test(test_hal_mock_reset_i2c_handle_clears_buffer),
    cmocka_unit_test(test_hal_mock_reset_i2c_handle_resets_instance),
    cmocka_unit_test(test_hal_mock_reset_i2c_handle_keeps_init),
    cmocka_unit_test(test_hal_mock_reset_i2c_handle_repeat_reset),
};

// HAL_I2C_Init Tests
const struct CMUnitTest hal_mock_i2c_init_tests[NUM_HAL_MOCK_INIT_TESTS] = {
    cmocka_unit_test(test_hal_mock_i2c_init_changes_state),
    cmocka_unit_test(test_hal_mock_i2c_init_changes_mode),
    cmocka_unit_test(test_hal_mock_i2c_init_from_error),
    cmocka_unit_test(test_hal_mock_i2c_init_left_unlocked),
    cmocka_unit_test(test_hal_mock_i2c_init_clears_buffer),
    cmocka_unit_test(test_hal_mock_i2c_init_keeps_init),
    cmocka_unit_test(test_hal_mock_i2c_init_resets_instance),
    cmocka_unit_test(test_hal_mock_i2c_init_already_initialized),
    cmocka_unit_test(test_hal_mock_i2c_init_repeat_init),
};

// HAL_I2C_DeInit Tests
const struct CMUnitTest hal_mock_i2c_deinit_tests[NUM_HAL_MOCK_DEINIT_TESTS] = {
    cmocka_unit_test(test_hal_mock_i2c_deinit_changes_state),
    cmocka_unit_test(test_hal_mock_i2c_deinit_changes_mode),
    cmocka_unit_test(test_hal_mock_i2c_deinit_from_error),
    cmocka_unit_test(test_hal_mock_i2c_deinit_left_unlocked),
    cmocka_unit_test(test_hal_mock_i2c_deinit_clears_buffer),
    cmocka_unit_test(test_hal_mock_i2c_deinit_keeps_init),
    cmocka_unit_test(test_hal_mock_i2c_deinit_resets_instance),
    cmocka_unit_test(test_hal_mock_i2c_deinit_busy_peripheral),
    cmocka_unit_test(test_hal_mock_i2c_deinit_repeat_deinit),
};

// Required structs for I2C
I2C_TypeDef I2C1;
I2C_HandleTypeDef hi2c1;

// Helper function that sets an I2C handle to a default state
static void set_i2c_default_state(I2C_HandleTypeDef *hi2c, I2C_TypeDef *i2cInstance)
{
    if(hi2c == NULL || i2cInstance == NULL) {
        return;
    }

    // Default values for I2C instance, adjust as needed
    i2cInstance->DR = 0;
    i2cInstance->SR = 0;
    i2cInstance->OAR = 0;
    i2cInstance->ErrorFlags = HAL_I2C_ERROR_NONE;
    i2cInstance->TransactionInfo.TargetAddress = 0;
    i2cInstance->TransactionInfo.ReadWrite = 0;
    i2cInstance->TransactionInfo.DataSize = 0;

    // Assign the I2C instance to the handle
    hi2c->Instance = i2cInstance;

    // Default values for hi2c->Init, adjust as needed
    hi2c->Init.ClockSpeed = 100000;
    hi2c->Init.DutyCycle = 0;
    hi2c->Init.OwnAddress1 = 0;
    hi2c->Init.AddressingMode = 0;
    hi2c->Init.DualAddressMode = 0;
    hi2c->Init.OwnAddress2 = 0;
    hi2c->Init.GeneralCallMode = 0;
    hi2c->Init.NoStretchMode = 0;
    
    // Default values for all other hi2c fields, adjust as needed
    memset(hi2c->pBuff, 0, HAL_I2C_MOCK_BUFFER_SIZE);
    hi2c->XferSize = 0;
    hi2c->XferCount = 0;
    hi2c->Lock = HAL_UNLOCKED;
    hi2c->State = HAL_I2C_STATE_RESET;
    hi2c->Mode = HAL_I2C_MODE_NONE;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
}

// Run all I2C tests
void run_hal_mock_i2c_tests()
{
    int status = 0;
    
    status += cmocka_run_group_tests(common_i2c_checks_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_reset_i2c_handle_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_i2c_init_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_i2c_deinit_tests, NULL, NULL);
    /*
    status += cmocka_run_group_tests(hal_mock_master_transmit_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_master_receive_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_slave_transmit_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_slave_receive_tests, NULL, NULL);
    */

   assert_int_equal(status, 0);
}

// Test Case: Verify that common_i2c_checks passes for an appropriately configured I2C handle
void test_common_i2c_checks_returns_ok(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);

    // Act: Call a function that uses common_i2c_checks (Reset_I2C_Handle) with configured handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(&hi2c1);

    // Assert: The function should return HAL_OK with handler error code HAL_I2C_ERROR_NONE
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_NONE);
}

// Test Case: Verify that common_i2c_checks fails when a null I2C handle is passed
void test_common_i2c_checks_null_input(void **state)
{
    // Arrange: Initialize HAL
    hal_initialized = 1;

    // Act: Call a function that uses common_i2c_checks (Reset_I2C_Handle) with a null handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(NULL);

    // Assert: The function should return HAL_ERROR
    assert_int_equal(rc, HAL_ERROR);
}

// Test Case: Verify that common_i2c_checks fails when HAL is not initialized
void test_common_i2c_checks_no_hal_init(void **state)
{
    // Arrange: Uninitialize HAL and set I2C handle and instance to a default state
    hal_initialized = 0;
    set_i2c_default_state(&hi2c1, &I2C1);

    // Act: Call a function that uses common_i2c_checks (Reset_I2C_Handle) with configured handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(&hi2c1);

    // Assert: The function should return HAL_ERROR and with handler error code HAL_I2C_ERROR_HAL_UNITIALIZED
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_HAL_UNITIALIZED);
}

// Test Case: Verify that common_i2c_checks fails when an invalid I2C instance is passed
void test_common_i2c_checks_invalid_instance(void **state)
{
    // Arrange: Initialize HAL and set a I2C handler with a null instance
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.Instance = NULL;

    // Act: Call a function that uses common_i2c_checks (Reset_I2C_Handle) with configured handles
    HAL_StatusTypeDef rc1 = Reset_I2C_Handle(&hi2c1);

    // Assert: The function should return HAL_ERROR with handler error code HAL_I2C_ERROR_INVALID_INSTANCE
    assert_int_equal(rc1, HAL_ERROR);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_INVALID_INSTANCE);
}

// Test Case: Verify that common_i2c_checks fails when the I2C handle is busy (indicated by the lock)
void test_common_i2c_checks_handle_busy(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state, then set the lock to HAL_LOCKED to indicate busyness
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.Lock = HAL_LOCKED;

    // Act: Call a function that uses common_i2c_checks (Reset_I2C_Handle) with configured handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(&hi2c1);

    // Assert: The function should return HAL_BUSY with handler error code HAL_I2C_ERROR_BUSY
    assert_int_equal(rc, HAL_BUSY);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_BUSY);
}

// Test Case: Verify that Reset_I2C_Handle changes the state of the I2C handle from HAL_I2C_STATE_READY to HAL_I2C_STATE_RESET
void test_hal_mock_reset_i2c_handle_changes_state(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state with state HAL_I2C_STATE_READY
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.State = HAL_I2C_STATE_READY;

    // Act: Call Reset_I2C_Handle with the configured handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(&hi2c1);

    // Assert: The function should return HAL_OK and the state of the handle should now be HAL_I2C_STATE_RESET
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.State, HAL_I2C_STATE_RESET);
}

// Test Case: Verify that Reset_I2C_Handle changes the mode of the I2C handle to HAL_I2C_MODE_NONE
void test_hal_mock_reset_i2c_handle_changes_mode(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state with different mode
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.Mode = HAL_I2C_MODE_MASTER;

    // Act: Call Reset_I2C_Handle with the configured handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(&hi2c1);

    // Assert: The function should return HAL_OK and the mode of the handle should now be HAL_I2C_MODE_NONE
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.Mode, HAL_I2C_MODE_NONE);
}

// Test Case: Verify that Reset_I2C_Handle resets correctly from an error state
void test_hal_mock_reset_i2c_handle_from_error(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state, then set the error code and error flag to a non-zero value
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.State = HAL_I2C_STATE_ERROR;
    hi2c1.ErrorCode = HAL_I2C_ERROR_BERR;
    I2C1.ErrorFlags = HAL_I2C_ERROR_BERR;

    // Act: Call Reset_I2C_Handle with the configured handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(&hi2c1);

    // Assert: The function should return HAL_OK and the error code and error flag should now be HAL_I2C_ERROR_NONE
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_NONE);
    assert_int_equal(I2C1.ErrorFlags, HAL_I2C_ERROR_NONE);
}

// Test Case: Verify that Reset_I2C_Handle leaves the I2C handle unlocked after use
void test_hal_mock_reset_i2c_handle_left_unlocked(void **state) 
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);

    // Act: Call Reset_I2C_Handle with the configured handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(&hi2c1);

    // Assert: The function should return HAL_OK and the handle should still be unlocked
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.Lock, HAL_UNLOCKED);
}

// Test Case: Verify that Reset_I2C_Handle clears the xfer buffer in the I2C handle
void test_hal_mock_reset_i2c_handle_clears_buffer(void **state) 
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state, then fill the buffer with non-zero values
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    // Fill the buffer with non-zero values to simulate existing data
    memset(hi2c1.pBuff, 0xAA, HAL_I2C_MOCK_BUFFER_SIZE);
    hi2c1.XferSize = HAL_I2C_MOCK_BUFFER_SIZE;
    hi2c1.XferCount = HAL_I2C_MOCK_BUFFER_SIZE;

    // Act: Call Reset_I2C_Handle with the configured handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(&hi2c1);

    // Assert: The function should return HAL_OK and the buffer, its size and its pointer should now be cleared
    assert_int_equal(rc, HAL_OK);
    // Check if buffer is cleared
    for (int i = 0; i < HAL_I2C_MOCK_BUFFER_SIZE; i++) {
        assert_int_equal(hi2c1.pBuff[i], 0);
    }
    assert_int_equal(hi2c1.XferSize, 0);
    assert_int_equal(hi2c1.XferCount, 0);
}

// Test Case: Verify that Reset_I2C_Handle resets the I2C instance to a default state
void test_hal_mock_reset_i2c_handle_resets_instance(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state, then fill the instance with non-zero values
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    // Fill the instance with non-zero values to simulate existing data
    I2C1.DR = 0xAA;
    I2C1.SR = 0xAA;
    I2C1.OAR = 0xAA;
    I2C1.ErrorFlags = HAL_I2C_ERROR_BERR;
    I2C1.TransactionInfo.TargetAddress = 0xAA;
    I2C1.TransactionInfo.ReadWrite = 0xAA;
    I2C1.TransactionInfo.DataSize = 0xAA;

    // Act: Call Reset_I2C_Handle with the configured handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(&hi2c1);

    // Assert: The function should return HAL_OK and the instance should now be reset
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(I2C1.DR, 0);
    assert_int_equal(I2C1.SR, 0);
    assert_int_equal(I2C1.OAR, 0);
    assert_int_equal(I2C1.ErrorFlags, HAL_I2C_ERROR_NONE);
    assert_int_equal(I2C1.TransactionInfo.TargetAddress, 0);
    assert_int_equal(I2C1.TransactionInfo.ReadWrite, 0);
    assert_int_equal(I2C1.TransactionInfo.DataSize, 0);
}

// Test Case: Verify that Reset_I2C_Handle maintains init settings
void test_hal_mock_reset_i2c_handle_keeps_init(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state with set Init
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.Init.ClockSpeed = 400000;
    hi2c1.Init.DutyCycle = 1;
    hi2c1.Init.OwnAddress1 = 0x33;
    hi2c1.Init.AddressingMode = 1;
    hi2c1.Init.DualAddressMode = 1;
    hi2c1.Init.OwnAddress2 = 0x44;
    hi2c1.Init.GeneralCallMode = 1;
    hi2c1.Init.NoStretchMode = 1;

    // Act: Call Reset_I2C_Handle with the configured handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(&hi2c1);

    // Assert: The function should return HAL_OK and the Init should remain unchanged
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.Init.ClockSpeed, 400000);
    assert_int_equal(hi2c1.Init.DutyCycle, 1);
    assert_int_equal(hi2c1.Init.OwnAddress1, 0x33);
    assert_int_equal(hi2c1.Init.AddressingMode, 1);
    assert_int_equal(hi2c1.Init.DualAddressMode, 1);
    assert_int_equal(hi2c1.Init.OwnAddress2, 0x44);
    assert_int_equal(hi2c1.Init.GeneralCallMode, 1);
    assert_int_equal(hi2c1.Init.NoStretchMode, 1);
}

// Test Case: Verify that Reset_I2C_Handle can be called multiple times without breaking
void test_hal_mock_reset_i2c_handle_repeat_reset(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a non-default state
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.State = HAL_I2C_STATE_BUSY;
    hi2c1.Mode = HAL_I2C_MODE_MASTER;

    // Act: Repeatedly call Reset_I2C_Handle with the configured handle
    HAL_StatusTypeDef rc = Reset_I2C_Handle(&hi2c1);
    assert_int_equal(rc, HAL_OK);
    for (int i = 0; i < 5; i++) {
        rc = Reset_I2C_Handle(&hi2c1);
        assert_int_equal(rc, HAL_OK);
    }

    // Assert: Check if the handle remains in default state after repeated resets
    assert_int_equal(hi2c1.State, HAL_I2C_STATE_RESET);
    assert_int_equal(hi2c1.Mode, HAL_I2C_MODE_NONE);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_NONE);
}

// Test Case: Verify that HAL_I2C_Init changes the state of the I2C handle from HAL_I2C_STATE_RESET to HAL_I2C_STATE_READY
void test_hal_mock_i2c_init_changes_state(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.State = HAL_I2C_STATE_RESET;

    // Act: Call HAL_I2C_Init with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c1);

    // Assert: The function should return HAL_OK and the state of the handle should now be HAL_I2C_STATE_READY
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.State, HAL_I2C_STATE_READY);
}

// Test Case: Verify that HAL_I2C_Init changes the mode of the I2C handle to HAL_I2C_MODE_NONE
void test_hal_mock_i2c_init_changes_mode(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state with different mode
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.Mode = HAL_I2C_MODE_MASTER;
    
    // Act: Call HAL_I2C_Init with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c1);

    // Assert: The function should return HAL_OK and the mode of the handle should now be HAL_I2C_MODE_NONE
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.Mode, HAL_I2C_MODE_NONE);
}

// Test Case: Verify that HAL_I2C_Init initializes correctly from an error state
void test_hal_mock_i2c_init_from_error(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state, then set the error code and error flag to a non-zero value
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.State = HAL_I2C_STATE_ERROR;
    hi2c1.ErrorCode = HAL_I2C_ERROR_BERR;
    I2C1.ErrorFlags = HAL_I2C_ERROR_BERR;

    // Act: Call HAL_I2C_Init with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c1);

    // Assert: The function should return HAL_OK and the error code and error flag should now be HAL_I2C_ERROR_NONE
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_NONE);
    assert_int_equal(I2C1.ErrorFlags, HAL_I2C_ERROR_NONE);
}

// Test Case: Verify that HAL_I2C_Init leaves the I2C handle unlocked after use
void test_hal_mock_i2c_init_left_unlocked(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);

    // Act: Call HAL_I2C_Init with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c1);

    // Assert: The function should return HAL_OK and the handle should still be unlocked
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.Lock, HAL_UNLOCKED);
}

// Test Case: Verify that HAL_I2C_Init clears the xfer buffer in the I2C handle
void test_hal_mock_i2c_init_clears_buffer(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state, then fill the buffer with non-zero values
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    // Fill the buffer with non-zero values to simulate existing data
    memset(hi2c1.pBuff, 0xAA, HAL_I2C_MOCK_BUFFER_SIZE);
    hi2c1.XferSize = HAL_I2C_MOCK_BUFFER_SIZE;
    hi2c1.XferCount = HAL_I2C_MOCK_BUFFER_SIZE;

    // Act: Call HAL_I2C_Init with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c1);

    // Assert: The function should return HAL_OK and the buffer, its size and its pointer should now be cleared
    assert_int_equal(rc, HAL_OK);
    // Check if buffer is cleared
    for (int i = 0; i < HAL_I2C_MOCK_BUFFER_SIZE; i++) {
        assert_int_equal(hi2c1.pBuff[i], 0);
    }
    assert_int_equal(hi2c1.XferSize, 0);
    assert_int_equal(hi2c1.XferCount, 0);
}

// Test Case: Verify that HAL_I2C_Init maintians init settings
void test_hal_mock_i2c_init_keeps_init(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state with set Init
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.Init.ClockSpeed = 400000;
    hi2c1.Init.DutyCycle = 1;
    hi2c1.Init.OwnAddress1 = 0x33;
    hi2c1.Init.AddressingMode = 1;
    hi2c1.Init.DualAddressMode = 1;
    hi2c1.Init.OwnAddress2 = 0x44;
    hi2c1.Init.GeneralCallMode = 1;
    hi2c1.Init.NoStretchMode = 1;

    // Act: Call HAL_I2C_Init with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c1);

    // Assert: The function should return HAL_OK and the Init should remain unchanged
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.Init.ClockSpeed, 400000);
    assert_int_equal(hi2c1.Init.DutyCycle, 1);
    assert_int_equal(hi2c1.Init.OwnAddress1, 0x33);
    assert_int_equal(hi2c1.Init.AddressingMode, 1);
    assert_int_equal(hi2c1.Init.DualAddressMode, 1);
    assert_int_equal(hi2c1.Init.OwnAddress2, 0x44);
    assert_int_equal(hi2c1.Init.GeneralCallMode, 1);
    assert_int_equal(hi2c1.Init.NoStretchMode, 1);
}

// Test Case: Verify that HAL_I2C_Init resets the I2C instance to a default state
void test_hal_mock_i2c_init_resets_instance(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state, then fill the instance with non-zero values
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    // Fill the instance with non-zero values to simulate existing data
    I2C1.DR = 0xAA;
    I2C1.SR = 0xAA;
    I2C1.OAR = 0xAA;
    I2C1.ErrorFlags = HAL_I2C_ERROR_BERR;
    I2C1.TransactionInfo.TargetAddress = 0xAA;
    I2C1.TransactionInfo.ReadWrite = 0xAA;
    I2C1.TransactionInfo.DataSize = 0xAA;

    // Act: Call HAL_I2C_Init with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c1);

    // Assert: The function should return HAL_OK and the instance should now be reset
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(I2C1.DR, 0);
    assert_int_equal(I2C1.SR, 0);
    assert_int_equal(I2C1.OAR, 0);
    assert_int_equal(I2C1.ErrorFlags, HAL_I2C_ERROR_NONE);
    assert_int_equal(I2C1.TransactionInfo.TargetAddress, 0);
    assert_int_equal(I2C1.TransactionInfo.ReadWrite, 0);
    assert_int_equal(I2C1.TransactionInfo.DataSize, 0);
}

// Test Case: Verify that HAL_I2C_Init returns error when the I2C handle is already initialized (state isn't HAL_I2C_STATE_RESET or HAL_I2C_STATE_ERROR)
void test_hal_mock_i2c_init_already_initialized(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state with different state
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.State = HAL_I2C_STATE_READY;

    // Act: Call HAL_I2C_Init with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c1);

    // Assert: The function should return HAL_ERROR and the error code should be HAL_I2C_ERROR_ALREADY_INITIALIZED
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_ALREADY_INITIALIZED);
}

// Test Case: Verify that HAL_I2C_Init can be called multiple times without breaking
void test_hal_mock_i2c_init_repeat_init(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a non-default state
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.State = HAL_I2C_STATE_ERROR;
    hi2c1.Mode = HAL_I2C_MODE_MASTER;

    // Act: Repeatedly call HAL_I2C_Init with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c1);
    assert_int_equal(rc, HAL_OK);
    for(int i = 0; i < 5; i++) {
        rc = HAL_I2C_Init(&hi2c1);
        assert_int_equal(rc, HAL_ERROR);
    }

    // Assert: Check if the handle changes on the first init but failed to initialize again
    assert_int_equal(hi2c1.State, HAL_I2C_STATE_READY);
    assert_int_equal(hi2c1.Mode, HAL_I2C_MODE_NONE);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_ALREADY_INITIALIZED);
}

// Test Case: Verify that HAL_I2C_DeInit changes the state of the I2C handle
void test_hal_mock_i2c_deinit_changes_state(void **state) 
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state with state HAL_I2C_STATE_READY
    hal_initialized = 1;

    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.State = HAL_I2C_STATE_READY;

    // Act: Call HAL_I2C_DeInit to deinitialize the I2C handle
    HAL_StatusTypeDef rc = HAL_I2C_DeInit(&hi2c1);

    // Assert: Check if the function returns HAL_OK and the state changes to RESET
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.State, HAL_I2C_STATE_RESET);
}

// Test Case: Verify that HAL_I2C_DeInit changes the mode of the I2C handle
void test_hal_mock_i2c_deinit_changes_mode(void **state) 
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state with mode HAL_I2C_MODE_MASTER
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.Mode = HAL_I2C_MODE_MASTER;

    // Act: Call HAL_I2C_DeInit to deinitialize the I2C handle
    HAL_StatusTypeDef rc = HAL_I2C_DeInit(&hi2c1);

    // Assert: Check if the function returns HAL_OK and the mode is changed to NONE
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.Mode, HAL_I2C_MODE_NONE);
}

// Test Case: Verify that HAL_I2C_DeInit cannot successfully be called when the handle is in an error state
void test_hal_mock_i2c_deinit_from_error(void **state) 
{
    // Arrange: Initialize HAL and set I2C handle and instance to an error state
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.State = HAL_I2C_STATE_ERROR;
    hi2c1.ErrorCode = HAL_I2C_ERROR_BERR;
    hi2c1.Instance->ErrorFlags = HAL_I2C_ERROR_BERR;

    // Act: Call HAL_I2C_DeInit to deinitialize the I2C handle
    HAL_StatusTypeDef rc = HAL_I2C_DeInit(&hi2c1);

    // Assert: Check if the function returns HAL_BUSY and the state changes to ERROR
    assert_int_equal(rc, HAL_BUSY);
    assert_int_equal(hi2c1.State, HAL_I2C_STATE_ERROR);
}

// Test Case: Verify that HAL_I2C_DeInit leaves the I2C handle unlocked
void test_hal_mock_i2c_deinit_left_unlocked(void **state) 
{
    // Arrange: Initialize HAL and set I2C handle to a default state (unlocked by default)
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);

    // Act: Call HAL_I2C_DeInit to deinitialize the I2C handle
    HAL_StatusTypeDef rc = HAL_I2C_DeInit(&hi2c1);

    // Assert: Check if the function returns HAL_OK and the lock is set to UNLOCKED
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.Lock, HAL_UNLOCKED);
}

// Test Case: Verify that HAL_I2C_DeInit clears the I2C transfer buffer
void test_hal_mock_i2c_deinit_clears_buffer(void **state) 
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state, then fill the buffer with non-zero values
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    memset(hi2c1.pBuff, 0xAA, HAL_I2C_MOCK_BUFFER_SIZE); // Fill buffer with dummy data
    hi2c1.XferSize = HAL_I2C_MOCK_BUFFER_SIZE;
    hi2c1.XferCount = HAL_I2C_MOCK_BUFFER_SIZE;

    // Act: Call HAL_I2C_DeInit to deinitialize the I2C handle
    HAL_StatusTypeDef rc = HAL_I2C_DeInit(&hi2c1);

    // Assert: The function should return HAL_OK and the buffer, its size and its pointer should now be cleared
    assert_int_equal(rc, HAL_OK);
    for (size_t i = 0; i < HAL_I2C_MOCK_BUFFER_SIZE; ++i) {
        assert_int_equal(hi2c1.pBuff[i], 0);
    }
    assert_int_equal(hi2c1.XferSize, 0);
    assert_int_equal(hi2c1.XferCount, 0);
}

// Test Case: Verify that HAL_I2C_DeInit maintains I2C init settings
void test_hal_mock_i2c_deinit_keeps_init(void **state) 
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state with set Init
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.Init.ClockSpeed = 400000;
    hi2c1.Init.DutyCycle = 1;
    hi2c1.Init.OwnAddress1 = 0x33;
    hi2c1.Init.AddressingMode = 1;
    hi2c1.Init.DualAddressMode = 1;
    hi2c1.Init.OwnAddress2 = 0x44;
    hi2c1.Init.GeneralCallMode = 1;
    hi2c1.Init.NoStretchMode = 1;

    // Act: Call HAL_I2C_DeInit with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_DeInit(&hi2c1);

    // Assert: The function should return HAL_OK and the Init should remain unchanged
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c1.Init.ClockSpeed, 400000);
    assert_int_equal(hi2c1.Init.DutyCycle, 1);
    assert_int_equal(hi2c1.Init.OwnAddress1, 0x33);
    assert_int_equal(hi2c1.Init.AddressingMode, 1);
    assert_int_equal(hi2c1.Init.DualAddressMode, 1);
    assert_int_equal(hi2c1.Init.OwnAddress2, 0x44);
    assert_int_equal(hi2c1.Init.GeneralCallMode, 1);
    assert_int_equal(hi2c1.Init.NoStretchMode, 1);
}

// Test Case: Verify that HAL_I2C_DeInit resets the I2C instance to a default state
void test_hal_mock_i2c_deinit_resets_instance(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a default state, then fill the instance with non-zero values
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    // Fill the instance with non-zero values to simulate existing data
    I2C1.DR = 0xAA;
    I2C1.SR = 0xAA;
    I2C1.OAR = 0xAA;
    I2C1.ErrorFlags = HAL_I2C_ERROR_BERR;
    I2C1.TransactionInfo.TargetAddress = 0xAA;
    I2C1.TransactionInfo.ReadWrite = 0xAA;
    I2C1.TransactionInfo.DataSize = 0xAA;

    // Act: Call HAL_I2C_Init with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_DeInit(&hi2c1);

    // Assert: The function should return HAL_OK and the instance should now be reset
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(I2C1.DR, 0);
    assert_int_equal(I2C1.SR, 0);
    assert_int_equal(I2C1.OAR, 0);
    assert_int_equal(I2C1.ErrorFlags, HAL_I2C_ERROR_NONE);
    assert_int_equal(I2C1.TransactionInfo.TargetAddress, 0);
    assert_int_equal(I2C1.TransactionInfo.ReadWrite, 0);
    assert_int_equal(I2C1.TransactionInfo.DataSize, 0);
}

// Test Case: Verify that HAL_I2C_DeInit fails for a busy peripheral
void test_hal_mock_i2c_deinit_busy_peripheral(void **state) 
{
    // Arrange: Initialize HAL and set I2C handle to a default state with busy state (not RESET or READY) 
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.State = HAL_I2C_STATE_BUSY;

    // Act: Call HAL_I2C_DeInit on a busy handle
    HAL_StatusTypeDef rc = HAL_I2C_DeInit(&hi2c1);

    // Assert: Check if the function returns HAL_BUSY and the error code is set to HAL_I2C_ERROR_BUSY
    assert_int_equal(rc, HAL_BUSY);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_BUSY);
}

void test_hal_mock_i2c_deinit_repeat_deinit(void **state)
{
    // Arrange: Initialize HAL and set I2C handle and instance to a non-default state
    hal_initialized = 1;
    set_i2c_default_state(&hi2c1, &I2C1);
    hi2c1.State = HAL_I2C_STATE_READY;
    hi2c1.Mode = HAL_I2C_MODE_MASTER;

    // Act: Repeatedly call HAL_I2C_DeInit with the configured handle
    HAL_StatusTypeDef rc = HAL_I2C_DeInit(&hi2c1);
    assert_int_equal(rc, HAL_OK);
    for(int i = 0; i < 5; i++) {
        rc = HAL_I2C_DeInit(&hi2c1);
        assert_int_equal(rc, HAL_OK);
    }

    // Assert: Check if the handle maintains its value from the first deinitialization after repeated deinitializations
    assert_int_equal(hi2c1.State, HAL_I2C_STATE_RESET);
    assert_int_equal(hi2c1.Mode, HAL_I2C_MODE_NONE);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_NONE);
}