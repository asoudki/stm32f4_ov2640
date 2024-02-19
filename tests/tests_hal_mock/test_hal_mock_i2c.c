#include "test_hal_mock_i2c.h"

// Definition of test arrays

// common_i2c_checks Tests
const struct CMUnitTest common_i2c_checks_tests[NUM_COMMON_I2C_CHECKS_TESTS] = {
    cmocka_unit_test(test_common_i2c_checks_returns_ok),
    cmocka_unit_test(test_common_i2c_checks_null_input),
    cmocka_unit_test(test_common_i2c_checks_no_hal_init),
};

// common_i2c_master_transaction_checks Tests
const struct CMUnitTest common_master_transaction_checks_tests[NUM_COMMON_MASTER_TRANSACTION_CHECKS_TESTS] = {
    cmocka_unit_test(test_common_i2c_master_transaction_checks_returns_ok),
    cmocka_unit_test(test_common_i2c_master_transaction_checks_null_pdata),
    cmocka_unit_test(test_common_i2c_master_transaction_checks_non_ready_state),
    cmocka_unit_test(test_common_i2c_master_transaction_checks_bad_slave),
};

// Set_I2C_Mock_Slave Tests
const struct CMUnitTest set_i2c_mock_slave_tests[NUM_SET_I2C_MOCK_SLAVE_TESTS] = {
    cmocka_unit_test(test_set_i2c_mock_slave_sets_values),
};

// HAL_I2C_Init Tests
const struct CMUnitTest hal_i2c_init_tests[NUM_HAL_I2C_INIT_TESTS] = {
    cmocka_unit_test(test_hal_i2c_init_sets_values),
};

// HAL_I2C_DeInit Tests
const struct CMUnitTest hal_i2c_deinit_tests[NUM_HAL_I2C_DEINIT_TESTS] = {
    cmocka_unit_test(test_hal_i2c_deinit_sets_values),
};

// HAL_I2C_Master_Transmit Tests
const struct CMUnitTest hal_i2c_master_transmit_tests[NUM_HAL_I2C_MASTER_TRANSMIT_TESTS] = {
    cmocka_unit_test(test_hal_i2c_master_transmit_transfers_data),
    cmocka_unit_test(test_hal_i2c_master_transmit_sets_state),
};

// HAL_I2C_Master_Receive Tests
const struct CMUnitTest hal_mock_master_receive_tests[NUM_HAL_I2C_MASTER_RECEIVE_TESTS] = {
    cmocka_unit_test(test_hal_i2c_master_receive_transfers_data),
    cmocka_unit_test(test_hal_i2c_master_receive_sets_state),
};

// Run all I2C tests
void run_hal_mock_i2c_tests()
{
    int status = 0;
    
    status += cmocka_run_group_tests(common_i2c_checks_tests, NULL, NULL);
    status += cmocka_run_group_tests(common_master_transaction_checks_tests, NULL, NULL);
    status += cmocka_run_group_tests(set_i2c_mock_slave_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_i2c_init_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_i2c_deinit_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_i2c_master_transmit_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_master_receive_tests, NULL, NULL);

   assert_int_equal(status, 0);
}

// Test Case: Verify that common_i2c_checks passes for an appropriately configured I2C handle
void test_common_i2c_checks_returns_ok(void **state)
{
    // Arrange: Initialize HAL and create I2C handle to pass in
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;

    // Act: Call a function that uses the private common_i2c_checks function
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c);

    // Assert: The function should succeed
    assert_int_equal(rc, HAL_OK);
}

// Test Case: Verify that common_i2c_checks fails for a NULL I2C handle
void test_common_i2c_checks_null_input(void **state)
{
    // Arrange: Initialize HAL and create I2C handle to pass in
    hal_initialized = 1;
    I2C_HandleTypeDef *hi2c = NULL;

    // Act: Call a function that uses the private common_i2c_checks function
    HAL_StatusTypeDef rc = HAL_I2C_Init(hi2c);

    // Assert: common_i2c_checks should catch the null input and return an error
    assert_int_equal(rc, HAL_ERROR);
}

// Test Case: Verify that common_i2c_checks fails for an uninitialized HAL
void test_common_i2c_checks_no_hal_init(void **state)
{
    // Arrange: Uninitialize HAL and create I2C handle to pass in
    hal_initialized = 0;
    I2C_HandleTypeDef hi2c;

    // Act: Call a function that uses the private common_i2c_checks function
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c);

    // Assert: common_i2c_checks should catch the uninitialized HAL and return an error
    assert_int_equal(rc, HAL_ERROR);
}

// Test Case: Verify that common_i2c_master_transaction_checks passes for an appropriately configured I2C handle
void test_common_i2c_master_transaction_checks_returns_ok(void **state)
{
    // Arrange: Initialize HAL, create a I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    uint8_t pData[10];
    uint16_t DevAddress = 0x01;
    uint8_t Slave[10];
    uint16_t Size = 10;

    HAL_I2C_Init(&hi2c);
    Set_I2C_Mock_Slave(&hi2c, DevAddress, Slave, Size);

    // Act: Call a function that uses the private common_i2c_master_transaction_checks function
    HAL_StatusTypeDef rc = HAL_I2C_Master_Transmit(&hi2c, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: The function should succeed
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
}

// Test Case: Verify that common_i2c_master_transaction_checks fails for a null slave pointer
void test_common_i2c_master_transaction_checks_null_pdata(void **state)
{
    // Arrange: Initialize HAL, create a I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    uint8_t pData[10];
    uint16_t DevAddress = 0x01;
    uint8_t Slave[10];
    uint16_t Size = 10;

    HAL_I2C_Init(&hi2c);
    Set_I2C_Mock_Slave(&hi2c, DevAddress, Slave, Size);

    // Act: Call a function that uses the private common_i2c_master_transaction_checks function, but pass a null slave pointer
    HAL_StatusTypeDef rc = HAL_I2C_Master_Transmit(&hi2c, DevAddress, NULL, Size, HAL_MAX_DELAY);

    // Assert: common_i2c_master_transaction_checks should catch the null slave pointer and return an error
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NULL_PARAM);
}

// Test Case: Verify that common_i2c_master_transaction_checks fails for all non-ready states
void test_common_i2c_master_transaction_checks_non_ready_state(void **state)
{
    // Arrange: Initialize HAL, create I2C handles and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c1, hi2c2, hi2c3;
    uint8_t pData[10];
    uint16_t DevAddress = 0x01;
    uint8_t Slave[10];
    uint16_t Size = 10;

    hi2c1.State = HAL_I2C_STATE_RESET;
    hi2c2.State = HAL_I2C_STATE_BUSY;
    hi2c3.State = HAL_I2C_STATE_ERROR;

    Set_I2C_Mock_Slave(&hi2c1, DevAddress, Slave, Size);
    Set_I2C_Mock_Slave(&hi2c2, DevAddress, Slave, Size);
    Set_I2C_Mock_Slave(&hi2c3, DevAddress, Slave, Size);

    // Act: Call a function that uses the private common_i2c_master_transaction_checks function
    HAL_StatusTypeDef rc1 = HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc2 = HAL_I2C_Master_Transmit(&hi2c2, DevAddress, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc3 = HAL_I2C_Master_Transmit(&hi2c3, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: common_i2c_master_transaction_checks should catch the non-ready states and return corresponding errors
    assert_int_equal(rc1, HAL_ERROR);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_UNINITIALIZED);
    assert_int_equal(rc2, HAL_BUSY);
    assert_int_equal(hi2c2.ErrorCode, HAL_I2C_ERROR_BUSY);
    assert_int_equal(rc3, HAL_ERROR);
    assert_int_equal(hi2c3.ErrorCode, HAL_I2C_ERROR_FAILSTATE);
}

// Test Case: Verify that common_i2c_master_transaction_checks fails for a bad slave
void test_common_i2c_master_transaction_checks_bad_slave(void **state)
{
    // Arrange: Initialize HAL, create I2C handles with improperly registered slaves
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c1, hi2c2, hi2c3, hi2c4;
    uint8_t pData[10];
    uint16_t DevAddress = 0x01;
    uint8_t Slave[10];
    uint16_t Size = 10;

    // Slave isn't set
    HAL_I2C_Init(&hi2c1);

    // Slave is set, but with null slave address
    HAL_I2C_Init(&hi2c2);
    Set_I2C_Mock_Slave(&hi2c2, DevAddress, NULL, Size);

    // Slave is set, but with a different size
    HAL_I2C_Init(&hi2c3);
    Set_I2C_Mock_Slave(&hi2c3, DevAddress, Slave, Size + 1);

    // Slave is set properly, but a different address will be pased into the transmit
    HAL_I2C_Init(&hi2c4);
    Set_I2C_Mock_Slave(&hi2c4, DevAddress, Slave, Size);

    // Act: Call master transmit with each handle
    HAL_StatusTypeDef rc1 = HAL_I2C_Master_Transmit(&hi2c1, hi2c1.XferAddress, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc2 = HAL_I2C_Master_Transmit(&hi2c2, hi2c2.XferAddress, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc3 = HAL_I2C_Master_Transmit(&hi2c3, hi2c3.XferAddress, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc4 = HAL_I2C_Master_Transmit(&hi2c4, DevAddress + 1, pData, Size, HAL_MAX_DELAY);

    // Assert: common_i2c_master_transaction_checks should catch each combination of bad slave and return corresponding errors
    assert_int_equal(rc1, HAL_ERROR);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_BAD_SLAVE);
    assert_int_equal(rc2, HAL_ERROR);
    assert_int_equal(hi2c2.ErrorCode, HAL_I2C_ERROR_BAD_SLAVE);
    assert_int_equal(rc3, HAL_ERROR);
    assert_int_equal(hi2c3.ErrorCode, HAL_I2C_ERROR_BAD_SLAVE);
    assert_int_equal(rc4, HAL_ERROR);
    assert_int_equal(hi2c4.ErrorCode, HAL_I2C_ERROR_BAD_SLAVE);
}

// Test Case: Verify that Set_I2C_Mock_Slave sets the slave device for the current master transaction
void test_set_i2c_mock_slave_sets_values(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    uint8_t Slave[10];
    uint16_t DevAddress = 0x01;
    uint8_t pData[10];
    uint16_t Size = 10;

    HAL_I2C_Init(&hi2c);

    // Act: Set the I2C slave device for the current master transaction
    HAL_StatusTypeDef rc = Set_I2C_Mock_Slave(&hi2c, DevAddress, Slave, Size);

    // Assert: The slave device should be set for the current master transaction
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.XferAddress, DevAddress);
    assert_int_equal(hi2c.XferBuffPtr, Slave);
    assert_int_equal(hi2c.XferSize, Size);
}

// Test Case: Verify that HAL_I2C_Init sets the I2C handle to a ready state
void test_hal_i2c_init_sets_values(void **state)
{
    // Arrange: Initialize HAL and create a I2C handle with values different from initialized
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    hi2c.State = HAL_I2C_STATE_RESET;
    hi2c.ErrorCode = HAL_I2C_ERROR_UNINITIALIZED;
    hi2c.XferAddress = 0x01;
    hi2c.XferSize = 3;

    // Act: Call HAL_I2C_Init
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c);

    // Assert: The I2C handle should be set to a ready state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.State, HAL_I2C_STATE_READY);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
    assert_int_equal(hi2c.XferAddress, 0);
    assert_int_equal(hi2c.XferBuffPtr, NULL);
    assert_int_equal(hi2c.XferSize, 0);
}

// Test Case: Verify that HAL_I2C_DeInit sets the I2C handle to a reset state
void test_hal_i2c_deinit_sets_values(void **state)
{
    // Arrange: Initialize HAL and create a I2C handle with values different from reset
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    hi2c.State = HAL_I2C_STATE_READY;
    hi2c.ErrorCode = HAL_I2C_ERROR_NONE;
    hi2c.XferAddress = 0x01;
    hi2c.XferSize = 3;

    // Act: Call HAL_I2C_DeInit
    HAL_StatusTypeDef rc = HAL_I2C_DeInit(&hi2c);

    // Assert: The I2C handle should be set to a reset state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.State, HAL_I2C_STATE_RESET);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
    assert_int_equal(hi2c.XferAddress, 0);
    assert_int_equal(hi2c.XferBuffPtr, NULL);
    assert_int_equal(hi2c.XferSize, 0);
}

// Test Case: Verify that HAL_I2C_Master_Transmit transfers data to the slave device
void test_hal_i2c_master_transmit_transfers_data(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    uint8_t Slave[10];
    uint16_t DevAddress = 0x01;
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    HAL_I2C_Init(&hi2c);
    Set_I2C_Mock_Slave(&hi2c, DevAddress, Slave, Size);

    // Act: Call HAL_I2C_Master_Transmit
    HAL_StatusTypeDef rc = HAL_I2C_Master_Transmit(&hi2c, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred to the slave device
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(Slave, pData, Size);
}

// Test Case: Verify that HAL_I2C_Master_Transmit sets the I2C handle to a ready state afterwards
void test_hal_i2c_master_transmit_sets_state(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    uint8_t Slave[10];
    uint16_t DevAddress = 0x01;
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    HAL_I2C_Init(&hi2c);
    Set_I2C_Mock_Slave(&hi2c, DevAddress, Slave, Size);

    // Act: Call HAL_I2C_Master_Transmit
    HAL_StatusTypeDef rc = HAL_I2C_Master_Transmit(&hi2c, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: The I2C handle should be set to a ready state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.State, HAL_I2C_STATE_READY);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
}

// Test Case: Verify that HAL_I2C_Master_Receive transfers data from the slave device
void test_hal_i2c_master_receive_transfers_data(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    uint8_t Slave[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t DevAddress = 0x01;
    uint8_t pData[10];
    uint16_t Size = 10;

    HAL_I2C_Init(&hi2c);
    Set_I2C_Mock_Slave(&hi2c, DevAddress, Slave, Size);

    // Act: Call HAL_I2C_Master_Receive
    HAL_StatusTypeDef rc = HAL_I2C_Master_Receive(&hi2c, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred from the slave device
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(Slave, pData, Size);
}

// Test Case: Verify that HAL_I2C_Master_Receive sets the I2C handle to a ready state afterwards
void test_hal_i2c_master_receive_sets_state(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    uint8_t Slave[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t DevAddress = 0x01;
    uint8_t pData[10];
    uint16_t Size = 10;

    HAL_I2C_Init(&hi2c);
    Set_I2C_Mock_Slave(&hi2c, DevAddress, Slave, Size);

    // Act: Call HAL_I2C_Master_Receive
    HAL_StatusTypeDef rc = HAL_I2C_Master_Receive(&hi2c, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: The I2C handle should be set to a ready state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.State, HAL_I2C_STATE_READY);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
}