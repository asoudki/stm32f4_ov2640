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
    cmocka_unit_test(test_common_i2c_master_transaction_checks_msg_too_big),
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
    cmocka_unit_test(test_hal_i2c_master_transmit_sets_values),
    cmocka_unit_test(test_hal_i2c_master_transmit_timeout),
};

// HAL_I2C_Master_Receive Tests
const struct CMUnitTest hal_mock_master_receive_tests[NUM_HAL_I2C_MASTER_RECEIVE_TESTS] = {
    cmocka_unit_test(test_hal_i2c_master_receive_transfers_data),
    cmocka_unit_test(test_hal_i2c_master_receive_sets_values),
    cmocka_unit_test(test_hal_i2c_master_receive_timeout),
};

// Mock_I2C_Slave_Transmit Tests
const struct CMUnitTest mock_i2c_slave_transmit_tests[NUM_MOCK_I2C_SLAVE_TRANSMIT_TESTS] = {
    cmocka_unit_test(test_mock_i2c_slave_transmit_transfers_data),
    cmocka_unit_test(test_mock_i2c_slave_transmit_sets_values),
    cmocka_unit_test(test_mock_i2c_slave_transmit_timeout),
};

// Mock_I2C_Slave_Receive Tests
const struct CMUnitTest mock_i2c_slave_receive_tests[NUM_MOCK_I2C_SLAVE_RECEIVE_TESTS] = {
    cmocka_unit_test(test_mock_i2c_slave_receive_transfers_data),
    cmocka_unit_test(test_mock_i2c_slave_receive_sets_values),
    cmocka_unit_test(test_mock_i2c_slave_receive_timeout),
};

// Run all I2C tests
void run_hal_mock_i2c_tests()
{
    int status = 0;
    
    status += cmocka_run_group_tests(common_i2c_checks_tests, NULL, NULL);
    status += cmocka_run_group_tests(common_master_transaction_checks_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_i2c_init_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_i2c_deinit_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_i2c_master_transmit_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_master_receive_tests, NULL, NULL);
    status += cmocka_run_group_tests(mock_i2c_slave_transmit_tests, NULL, NULL);
    status += cmocka_run_group_tests(mock_i2c_slave_receive_tests, NULL, NULL);

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
    hi2c.State = HAL_I2C_STATE_READY;
    
    uint16_t DevAddress = 0x01;
    uint8_t pData[10];
    uint16_t Size = 10;

    // Act: Call a function that uses the private common_i2c_master_transaction_checks function
    HAL_StatusTypeDef rc = HAL_I2C_Master_Transmit(&hi2c, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: The function should succeed
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
}

// Test Case: Verify that common_i2c_master_transaction_checks fails for a null pData pointer
void test_common_i2c_master_transaction_checks_null_pdata(void **state)
{
    // Arrange: Initialize HAL, create a I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    hi2c.State = HAL_I2C_STATE_READY;
    
    uint16_t DevAddress = 0x01;
    uint8_t pData[10];
    uint16_t Size = 10;

    // Act: Call a function that uses the private common_i2c_master_transaction_checks function, but pass a null pData pointer
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
    I2C_HandleTypeDef hi2c1, hi2c2;
    hi2c1.State = HAL_I2C_STATE_RESET;
    hi2c2.State = HAL_I2C_STATE_ERROR;

    uint16_t DevAddress = 0x01;
    uint8_t pData[10];
    uint16_t Size = 10;

    // Act: Call a function that uses the private common_i2c_master_transaction_checks function
    HAL_StatusTypeDef rc1 = HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc2 = HAL_I2C_Master_Transmit(&hi2c2, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: common_i2c_master_transaction_checks should catch the non-ready states and return corresponding errors
    assert_int_equal(rc1, HAL_ERROR);
    assert_int_equal(hi2c1.ErrorCode, HAL_I2C_ERROR_UNINITIALIZED);
    assert_int_equal(rc2, HAL_ERROR);
    assert_int_equal(hi2c2.ErrorCode, HAL_I2C_ERROR_FAILSTATE);
}

// Test Case: Verify that common_i2c_master_transaction_checks fails for message that's too big
void test_common_i2c_master_transaction_checks_msg_too_big(void **state)
{
    // Arrange: Initialize HAL, create I2C handles and prepare for transaction with an oversized message
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    hi2c.State = HAL_I2C_STATE_READY;

    uint16_t DevAddress = 0x01;
    uint8_t pData[MOCK_I2C_MAX_MSG_SIZE+1];
    uint16_t Size = MOCK_I2C_MAX_MSG_SIZE+1;

    // Act: Call a function that uses the private common_i2c_master_transaction_checks function
    HAL_StatusTypeDef rc = HAL_I2C_Master_Transmit(&hi2c, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: common_i2c_master_transaction_checks should catch oversized message and return a corresponding error
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_MSG_TOO_BIG);
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
    memset(hi2c.MsgBuff, 1, MOCK_I2C_MAX_MSG_SIZE);
    hi2c.MsgSize = MOCK_I2C_MAX_MSG_SIZE;

    // Act: Call HAL_I2C_Init
    HAL_StatusTypeDef rc = HAL_I2C_Init(&hi2c);

    // Assert: The I2C handle should be set to a ready state
    uint8_t expected_buff[MOCK_I2C_MAX_MSG_SIZE] = {0};

    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.State, HAL_I2C_STATE_READY);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
    assert_int_equal(hi2c.XferAddress, 0);
    assert_memory_equal(hi2c.MsgBuff, expected_buff, MOCK_I2C_MAX_MSG_SIZE);
    assert_int_equal(hi2c.MsgSize, 0);
}

// Test Case: Verify that HAL_I2C_DeInit sets the I2C handle to a reset state
void test_hal_i2c_deinit_sets_values(void **state)
{
    // Arrange: Initialize HAL and create a I2C handle with values different from reset
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    hi2c.State = HAL_I2C_STATE_READY;
    hi2c.ErrorCode = HAL_I2C_ERROR_UNINITIALIZED;
    hi2c.XferAddress = 0x01;
    memset(hi2c.MsgBuff, 1, MOCK_I2C_MAX_MSG_SIZE);
    hi2c.MsgSize = MOCK_I2C_MAX_MSG_SIZE;

    // Act: Call HAL_I2C_DeInit
    HAL_StatusTypeDef rc = HAL_I2C_DeInit(&hi2c);

    // Assert: The I2C handle should be set to a reset state
    uint8_t expected_buff[MOCK_I2C_MAX_MSG_SIZE] = {0};

    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.State, HAL_I2C_STATE_RESET);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
    assert_int_equal(hi2c.XferAddress, 0);
    assert_memory_equal(hi2c.MsgBuff, expected_buff, MOCK_I2C_MAX_MSG_SIZE);
    assert_int_equal(hi2c.MsgSize, 0);
}

// Test Case: Verify that HAL_I2C_Master_Transmit transfers data to the slave device
void test_hal_i2c_master_transmit_transfers_data(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    hi2c.State = HAL_I2C_STATE_READY;
    
    uint16_t DevAddress = 0x01;
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call HAL_I2C_Master_Transmit
    HAL_StatusTypeDef rc = HAL_I2C_Master_Transmit(&hi2c, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred to the slave device
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(hi2c.MsgBuff, pData, Size);
}

// Test Case: Verify that HAL_I2C_Master_Transmit sets expected values in the I2C handle
void test_hal_i2c_master_transmit_sets_values(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    hi2c.State = HAL_I2C_STATE_READY;
    
    uint16_t DevAddress = 0x01;
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call HAL_I2C_Master_Transmit
    HAL_StatusTypeDef rc = HAL_I2C_Master_Transmit(&hi2c, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: Values in the I2C handle should be set as expected according to current state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.State, HAL_I2C_STATE_BUSY_TX);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
    assert_int_equal(hi2c.XferAddress, DevAddress);
    assert_int_equal(hi2c.MsgSize, Size);
}

// Test Case: Verify that HAL_I2C_Transmit times out correctly when I2C stays at improper state
void test_hal_i2c_master_transmit_timeout(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    
    // Transmit should time out if I2C is held at a busy state (Another transaction is ongoing)
    hi2c.State = HAL_I2C_STATE_BUSY_TX;

    uint16_t DevAddress = 0x01;
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call HAL_I2C_Master_Transmit
    HAL_StatusTypeDef rc = HAL_I2C_Master_Transmit(&hi2c, DevAddress, pData, Size, 100);

    // Assert: The function should time out
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_TIMEOUT);
}

// Test Case: Verify that HAL_I2C_Master_Receive transfers data from the slave device
void test_hal_i2c_master_receive_transfers_data(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    hi2c.State = HAL_I2C_STATE_READY;
    
    uint16_t DevAddress = 0x01;
    uint8_t pData[10];
    uint16_t Size = 10;

    // Data sent from slave to receive
    hi2c.State = HAL_I2C_STATE_BUSY_RX;
    memset(hi2c.MsgBuff, 2, Size);

    // Act: Call HAL_I2C_Master_Receive
    HAL_StatusTypeDef rc = HAL_I2C_Master_Receive(&hi2c, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred from the slave device
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(pData, hi2c.MsgBuff, Size);
}

// Test Case: Verify that HAL_I2C_Master_Receive sets expected values in the I2C handle
void test_hal_i2c_master_receive_sets_values(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    hi2c.State = HAL_I2C_STATE_READY;
    
    uint16_t DevAddress = 0x01;
    uint8_t pData[10];
    uint16_t Size = 10;

    // Data sent from slave to receive
    hi2c.State = HAL_I2C_STATE_BUSY_RX;
    memset(hi2c.MsgBuff, 2, Size);

    // Act: Call HAL_I2C_Master_Receive
    HAL_StatusTypeDef rc = HAL_I2C_Master_Receive(&hi2c, DevAddress, pData, Size, HAL_MAX_DELAY);

    // Assert: Values in the I2C handle should be set as expected according to current state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.State, HAL_I2C_STATE_READY);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
    assert_int_equal(hi2c.XferAddress, DevAddress);
}

// Test Case: Verify that HAL_I2C_Receive times out correctly when I2C stays at improper state
void test_hal_i2c_master_receive_timeout(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    
    // Receive should time out if I2C is held at a ready state (No data sent from slave to receive)
    hi2c.State = HAL_I2C_STATE_READY;

    uint16_t DevAddress = 0x01;
    uint8_t pData[10];
    uint16_t Size = 10;

    // Act: Call HAL_I2C_Master_Receive
    HAL_StatusTypeDef rc = HAL_I2C_Master_Receive(&hi2c, DevAddress, pData, Size, 100);

    // Assert: The function should time out
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_TIMEOUT);
}

// Test Case: Verify that Mock_I2C_Slave_Transmit transfers data to the master device
void test_mock_i2c_slave_transmit_transfers_data(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    
    // Simulate 10-element message requested by HAL_I2C_Master_Receive
    hi2c.State = HAL_I2C_STATE_READY;
    hi2c.MsgSize = 10;
    
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call Mock_I2C_Slave_Transmit
    HAL_StatusTypeDef rc = Mock_I2C_Slave_Transmit(&hi2c, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred to the master device
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(hi2c.MsgBuff, pData, Size);
}

// Test Case: Verify that Mock_I2C_Slave_Transmit sets expected values in the I2C handle
void test_mock_i2c_slave_transmit_sets_values(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;

    // Simulate 10-element message requested by HAL_I2C_Master_Receive
    hi2c.State = HAL_I2C_STATE_READY;
    hi2c.MsgSize = 10;
    
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call Mock_I2C_Slave_Transmit
    HAL_StatusTypeDef rc = Mock_I2C_Slave_Transmit(&hi2c, pData, Size, HAL_MAX_DELAY);

    // Assert: Values in the I2C handle should be set as expected according to current state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.State, HAL_I2C_STATE_BUSY_TX);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
    assert_int_equal(hi2c.MsgSize, Size);
}

// Test Case: Verify that Mock_I2C_Slave_Transmit times out correctly when I2C stays at improper state
void test_mock_i2c_slave_transmit_timeout(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    
    // Transmit should time out if I2C is held at a busy state (Another transaction is ongoing)
    hi2c.State = HAL_I2C_STATE_BUSY_TX;

    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call Mock_I2C_Slave_Transmit
    HAL_StatusTypeDef rc = Mock_I2C_Slave_Transmit(&hi2c, pData, Size, 100);

    // Assert: The function should time out
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_TIMEOUT);
}

// Test Case: Verify that Mock_I2C_Slave_Transmit fails for a message size not promised by HAL_I2C_Master_Receive
void test_mock_i2c_slave_transmit_msg_size_mismatch(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    
    // Simulate 5-element message requested by HAL_I2C_Master_Receive
    hi2c.State = HAL_I2C_STATE_READY;
    hi2c.MsgSize = 5;
    
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call Mock_I2C_Slave_Transmit
    HAL_StatusTypeDef rc = Mock_I2C_Slave_Transmit(&hi2c, pData, Size, HAL_MAX_DELAY);

    // Assert: The function should return an error
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_SIZE_MISMATCH);
}

// Test Case: Verify that Mock_I2C_Slave_Receive transfers data from the master device
void test_mock_i2c_slave_receive_transfers_data(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    
    uint8_t pData[10];
    uint16_t Size = 10;

    // Data sent from master to receive
    hi2c.State = HAL_I2C_STATE_BUSY_TX;
    hi2c.MsgSize = 10;
    memset(hi2c.MsgBuff, 2, Size);

    // Act: Call Mock_I2C_Slave_Receive
    HAL_StatusTypeDef rc = Mock_I2C_Slave_Receive(&hi2c, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred from the master device
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(pData, hi2c.MsgBuff, Size);
}

// Test Case: Verify that Mock_I2C_Slave_Receive sets expected values in the I2C handle
void test_mock_i2c_slave_receive_sets_values(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    hi2c.State = HAL_I2C_STATE_READY;
    
    uint8_t pData[10];
    uint16_t Size = 10;

    // Data sent from master to receive
    hi2c.State = HAL_I2C_STATE_BUSY_TX;
    hi2c.MsgSize = 10;
    memset(hi2c.MsgBuff, 2, Size);

    // Act: Call Mock_I2C_Slave_Receive
    HAL_StatusTypeDef rc = Mock_I2C_Slave_Receive(&hi2c, pData, Size, HAL_MAX_DELAY);

    // Assert: Values in the I2C handle should be set as expected according to current state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hi2c.State, HAL_I2C_STATE_READY);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_NONE);
}

// Test Case: Verify that Mock_I2C_Slave_Receive times out correctly when I2C stays at improper state
void test_mock_i2c_slave_receive_timeout(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    
    // Receive should time out if I2C is held at a ready state (No data sent from master to receive)
    hi2c.State = HAL_I2C_STATE_READY;

    uint8_t pData[10];
    uint16_t Size = 10;
    hi2c.MsgSize = 10;

    // Act: Call Mock_I2C_Slave_Receive
    HAL_StatusTypeDef rc = Mock_I2C_Slave_Receive(&hi2c, pData, Size, 100);

    // Assert: The function should time out
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_TIMEOUT);
}

// Test Case: Verify that Mock_I2C_Slave_Receive fails for a message size not promised by HAL_I2C_Master_Transmit
void test_mock_i2c_slave_receive_msg_size_mismatch(void **state)
{
    // Arrange: Initialize HAL, create I2C handle and prepare for transaction
    hal_initialized = 1;
    I2C_HandleTypeDef hi2c;
    
    uint8_t pData[10];
    uint16_t Size = 10;

    // Data sent from master to receive
    hi2c.State = HAL_I2C_STATE_BUSY_TX;
    hi2c.MsgSize = 5;
    memset(hi2c.MsgBuff, 2, Size);

    // Act: Call Mock_I2C_Slave_Receive
    HAL_StatusTypeDef rc = Mock_I2C_Slave_Receive(&hi2c, pData, Size, HAL_MAX_DELAY);

    // Assert: The function should return an error
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hi2c.ErrorCode, HAL_I2C_ERROR_SIZE_MISMATCH);
}