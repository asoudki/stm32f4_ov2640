#include "test_hal_mock_spi.h"

// Definition of test arrays

// common_spi_checks Tests
const struct CMUnitTest common_spi_checks_tests[NUM_COMMON_SPI_CHECKS_TESTS] = {
    cmocka_unit_test(test_common_spi_checks_returns_ok),
    cmocka_unit_test(test_common_spi_checks_null_input),
    cmocka_unit_test(test_common_spi_checks_no_hal_init)
};

// common_spi_transaction_checks Tests
const struct CMUnitTest common_spi_transaction_checks_tests[NUM_COMMON_SPI_TRANSACTION_CHECKS_TESTS] = {
    cmocka_unit_test(test_common_spi_transaction_checks_returns_ok),
    cmocka_unit_test(test_common_spi_transaction_checks_null_pdata),
    cmocka_unit_test(test_common_spi_transaction_checks_non_ready_state),
    cmocka_unit_test(test_common_spi_master_transaction_checks_msg_too_big)
};

// HAL_SPI_Init Tests
const struct CMUnitTest hal_mock_spi_init_tests[NUM_HAL_MOCK_INIT_TESTS] = {
    cmocka_unit_test(test_hal_spi_init_sets_values)
};

// HAL_SPI_DeInit Tests
const struct CMUnitTest hal_mock_spi_deinit_tests[NUM_HAL_MOCK_DEINIT_TESTS] = {
    cmocka_unit_test(test_hal_spi_deinit_sets_values)
};

// HAL_SPI_Transmit Tests
const struct CMUnitTest hal_mock_transmit_tests[NUM_HAL_MOCK_TRANSMIT_TESTS] = {
    cmocka_unit_test(test_hal_spi_transmit_transfers_data),
    cmocka_unit_test(test_hal_spi_transmit_sets_values),
    cmocka_unit_test(test_hal_spi_transmit_timeout),
};

// HAL_SPI_Transmit_DMA Tests
const struct CMUnitTest hal_mock_transmit_dma_tests[NUM_HAL_MOCK_TRANSMIT_DMA_TESTS] = {
    cmocka_unit_test(test_hal_spi_transmit_dma_transfers_data),
    cmocka_unit_test(test_hal_spi_transmit_dma_sets_values),
};

// HAL_SPI_Receive Tests
const struct CMUnitTest hal_mock_receive_tests[NUM_HAL_MOCK_RECEIVE_TESTS] = {
    cmocka_unit_test(test_hal_spi_receive_transfers_data),
    cmocka_unit_test(test_hal_spi_receive_sets_values),
    cmocka_unit_test(test_hal_spi_receive_timeout),
};

// HAL_SPI_Receive_DMA Tests
const struct CMUnitTest hal_mock_receive_dma_tests[NUM_HAL_MOCK_RECEIVE_DMA_TESTS] = {
    cmocka_unit_test(test_hal_spi_receive_dma_transfers_data),
    cmocka_unit_test(test_hal_spi_receive_dma_sets_values),
};

// Mock_SPI_Slave_Transmit Tests
const struct CMUnitTest mock_spi_slave_transmit_tests[NUM_MOCK_SPI_SLAVE_TRANSMIT_TESTS] = {
    cmocka_unit_test(test_mock_spi_slave_transmit_transfers_data),
    cmocka_unit_test(test_mock_spi_slave_transmit_sets_values),
    cmocka_unit_test(test_mock_spi_slave_transmit_timeout),
    cmocka_unit_test(test_mock_spi_slave_transmit_size_mismatch),
};

// Mock_SPI_Slave_Receive Tests
const struct CMUnitTest mock_spi_slave_receive_tests[NUM_MOCK_SPI_SLAVE_RECEIVE_TESTS] = {
    cmocka_unit_test(test_mock_spi_slave_receive_transfers_data),
    cmocka_unit_test(test_mock_spi_slave_receive_sets_values),
    cmocka_unit_test(test_mock_spi_slave_receive_timeout),
    cmocka_unit_test(test_mock_spi_slave_receive_size_mismatch),
};

void run_hal_mock_spi_tests(void) {
    int status = 0;
    
    status += cmocka_run_group_tests(common_spi_checks_tests, NULL, NULL);
    status += cmocka_run_group_tests(common_spi_transaction_checks_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_spi_init_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_spi_deinit_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_transmit_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_transmit_dma_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_receive_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_receive_dma_tests, NULL, NULL);
    status += cmocka_run_group_tests(mock_spi_slave_transmit_tests, NULL, NULL);
    status += cmocka_run_group_tests(mock_spi_slave_receive_tests, NULL, NULL);

    assert_int_equal(status, 0);
}

// Test Case: Verify that common_spi_checks passes for an appropriately configured SPI handle
void test_common_spi_checks_returns_ok(void **state) {
    // Arrange: Initialize HAL and create SPI handle to pass in
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    // Act: Call a function that uses the private common_spi_checks
    HAL_StatusTypeDef rc = HAL_SPI_Init(&hspi);

    // Assert: The function should succeed
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
}

// Test Case: Verify that common_spi_checks fails for a null SPI handle
void test_common_spi_checks_null_input(void **state) {
    // Arrange: Initialize HAL and create SPI handle to pass in
    hal_initialized = 1;

    // Act: Call a function that uses the private common_spi_checks
    HAL_StatusTypeDef rc = HAL_SPI_Init(NULL);

    // Assert: common_spi_checks should catch the null input and return an error
    assert_int_equal(rc, HAL_ERROR);
}

// Test Case: Verify that common_spi_checks fails for an uninitialized HAL
void test_common_spi_checks_no_hal_init(void **state) {
    // Arrange: Uninitialize HAL and create SPI handle to pass in
    hal_initialized = 0;
    SPI_HandleTypeDef hspi;

    // Act: Call a function that uses the private common_spi_checks
    HAL_StatusTypeDef rc = HAL_SPI_Init(&hspi);

    // Assert: common_spi_checks should catch the uninitialized HAL and return an error
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_HAL_UNINITIALIZED);
}

// Test Case: Verify that common_spi_transaction_checks passes for an appropriately configured SPI handle
void test_common_spi_transaction_checks_returns_ok(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_READY;

    uint8_t pData[10];
    uint16_t Size = 10;

    // Act: Call a function that uses the private common_spi_transaction_checks
    HAL_StatusTypeDef rc = HAL_SPI_Transmit(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The function should succeed
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
}

// Test Case: Verify that common_spi_transaction_checks fails for a null pData pointer
void test_common_spi_transaction_checks_null_pdata(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_READY;

    uint8_t pData[10];
    uint16_t Size = 10;

    // Act: Call a function that uses the private common_spi_transaction_checks, but with a null pData pointer
    HAL_StatusTypeDef rc = HAL_SPI_Transmit(&hspi, NULL, Size, HAL_MAX_DELAY);

    // Assert: common_spi_transaction_checks should catch the null pData and return an error
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NULL_PARAM);
}

// Test Case: Verify that common_spi_transaction_checks fails for all non-ready states
void test_common_spi_transaction_checks_non_ready_state(void **state) {
    // Arrange: Initialize HAL, create uninitialized, busy and erronous SPI handles and prepare for transactions with them
    hal_initialized = 1;
    SPI_HandleTypeDef hspi1, hspi2;
    hspi1.State = HAL_SPI_STATE_RESET;
    hspi2.State = HAL_SPI_STATE_ERROR;

    uint8_t pData[10];
    uint16_t Size = 10;

    // Act: Call a function that uses the private common_spi_transaction_checks
    HAL_StatusTypeDef rc1 = HAL_SPI_Transmit(&hspi1, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc2 = HAL_SPI_Transmit(&hspi2, pData, Size, HAL_MAX_DELAY);

    // Assert: common_spi_transaction_checks should catch the non-ready states and return corresponding errors
    assert_int_equal(rc1, HAL_ERROR);
    assert_int_equal(hspi1.ErrorCode, HAL_SPI_ERROR_UNINITIALIZED);
    assert_int_equal(rc2, HAL_ERROR);
    assert_int_equal(hspi2.ErrorCode, HAL_SPI_ERROR_FAILSTATE);
}

// Test Case: Verify that common_spi_master_transaction_checks fails for message that's too big
void test_common_spi_master_transaction_checks_msg_too_big(void **state)
{
    // Arrange: Initialize HAL, create SPI handles and prepare for transaction with an oversized message
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_READY;

    uint8_t pData[MOCK_SPI_MAX_MSG_SIZE+1];
    uint16_t Size = MOCK_SPI_MAX_MSG_SIZE+1;

    // Act: Call a function that uses the private common_i2c_master_transaction_checks function
    HAL_StatusTypeDef rc = HAL_SPI_Transmit(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: common_spi_master_transaction_checks should catch oversized message and return a corresponding error
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_MSG_TOO_BIG);
}

// Test Case: Verify that HAL_SPI_Init sets the SPI handle to a ready state
void test_hal_spi_init_sets_values(void **state) {
    // Arrange: Initialize HAL and create a SPI handle with values different from initialized
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_RESET;
    hspi.ErrorCode = 32;
    memset(hspi.TxMsgBuff, 1, MOCK_SPI_MAX_MSG_SIZE);
    hspi.TxMsgSize = MOCK_SPI_MAX_MSG_SIZE;
    memset(hspi.RxMsgBuff, 2, MOCK_SPI_MAX_MSG_SIZE);
    hspi.RxMsgSize = MOCK_SPI_MAX_MSG_SIZE;

    // Act: Call HAL_SPI_Init
    HAL_StatusTypeDef rc = HAL_SPI_Init(&hspi);

    // Assert: The SPI handle should be set to a ready state
    uint8_t expected_buff[MOCK_SPI_MAX_MSG_SIZE] = {0};

    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_READY);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
    assert_memory_equal(hspi.TxMsgBuff, expected_buff, MOCK_SPI_MAX_MSG_SIZE);
    assert_int_equal(hspi.TxMsgSize, 0);
    assert_memory_equal(hspi.RxMsgBuff, expected_buff, MOCK_SPI_MAX_MSG_SIZE);
    assert_int_equal(hspi.RxMsgSize, 0);
}

// Test Case: Verify that HAL_SPI_DeInit sets the state of the SPI handle to reset
void test_hal_spi_deinit_sets_values(void **state) {
    // Arrange: Initialize HAL and create SPI handle with values different from reset
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_ERROR;
    hspi.ErrorCode = HAL_SPI_ERROR_HAL_UNINITIALIZED;
    memset(hspi.TxMsgBuff, 1, MOCK_SPI_MAX_MSG_SIZE);
    hspi.TxMsgSize = MOCK_SPI_MAX_MSG_SIZE;
    memset(hspi.RxMsgBuff, 2, MOCK_SPI_MAX_MSG_SIZE);
    hspi.RxMsgSize = MOCK_SPI_MAX_MSG_SIZE;

    // Act: Call HAL_SPI_DeInit
    HAL_StatusTypeDef rc = HAL_SPI_DeInit(&hspi);

    // Assert: The SPI handle should be set to a reset state
    uint8_t expected_buff[MOCK_SPI_MAX_MSG_SIZE] = {0};

    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_RESET);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
    assert_memory_equal(hspi.TxMsgBuff, expected_buff, MOCK_SPI_MAX_MSG_SIZE);
    assert_int_equal(hspi.TxMsgSize, 0);
    assert_memory_equal(hspi.RxMsgBuff, expected_buff, MOCK_SPI_MAX_MSG_SIZE);
    assert_int_equal(hspi.RxMsgSize, 0);
}

// Test Case: Verify that HAL_SPI_Transmit transfers data correctly
void test_hal_spi_transmit_transfers_data(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_READY;

    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call HAL_SPI_Transmit
    HAL_StatusTypeDef rc = HAL_SPI_Transmit(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred correctly
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(hspi.TxMsgBuff, pData, Size);
}

// Test Case: Verify that HAL_SPI_Transmit sets expected values in the SPI handle
void test_hal_spi_transmit_sets_values(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_READY;

    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call HAL_SPI_Transmit
    HAL_StatusTypeDef rc = HAL_SPI_Transmit(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: Values in SPI handle should be set correctly according to current state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_BUSY_TX);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
    assert_int_equal(hspi.TxMsgSize, Size);
}

// Test Case: Verify that HAL_SPI_Transmit times out correctly when SPI stays at improper state
void test_hal_spi_transmit_timeout(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    // Transmit should time out if SPI is held at a busy state (Another transaction is ongoing)
    hspi.State = HAL_SPI_STATE_BUSY_TX;

    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call HAL_SPI_Transmit
    HAL_StatusTypeDef rc = HAL_SPI_Transmit(&hspi, pData, Size, 100);

    // Assert: The function should time out
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_TIMEOUT);
}

// Test Case: Verify that HAL_SPI_Transmit_DMA transfers data correctly
void test_hal_spi_transmit_dma_transfers_data(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_READY;

    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call HAL_SPI_Transmit_DMA
    HAL_StatusTypeDef rc = HAL_SPI_Transmit_DMA(&hspi, pData, Size);

    // Assert: The data should be transferred correctly
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(hspi.TxMsgBuff, pData, Size);
}

// Test Case: Verify that HAL_SPI_Transmit_DMA sets expected values in the SPI handle
void test_hal_spi_transmit_dma_sets_values(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_READY;

    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call HAL_SPI_Transmit_DMA
    HAL_StatusTypeDef rc = HAL_SPI_Transmit_DMA(&hspi, pData, Size);

    // Assert: Values in SPI handle should be set correctly according to current state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_BUSY_TX);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
    assert_int_equal(hspi.TxMsgSize, Size);
}

// Test Case: Verify that HAL_SPI_Receive transfers data correctly
void test_hal_spi_receive_transfers_data(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    uint8_t pData[10];
    uint16_t Size = 10;

    // Data sent from slave to receive
    hspi.State = HAL_SPI_STATE_BUSY_RX;
    memset(hspi.RxMsgBuff, 2, Size);

    // Act: Call HAL_SPI_Receive
    HAL_StatusTypeDef rc = HAL_SPI_Receive(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred correctly
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(pData, hspi.RxMsgBuff, Size);
}

// Test Case: Verify that HAL_SPI_Receive sets expected values in the SPI handle
void test_hal_spi_receive_sets_values(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    uint8_t pData[10];
    uint16_t Size = 10;

    // Data sent from slave to receive
    hspi.State = HAL_SPI_STATE_BUSY_RX;
    memset(hspi.RxMsgBuff, 2, Size);
    
    // Act: Call HAL_SPI_Receive
    HAL_StatusTypeDef rc = HAL_SPI_Receive(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: Values in SPI handle should be set correctly according to current state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_READY);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
    assert_int_equal(hspi.RxMsgSize, 0);
}

// Test Case: Verify that HAL_SPI_Receive times out correctly when SPI stays at improper state
void test_hal_spi_receive_timeout(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    // Receive should time out if SPI is held at a ready state (No data sent from slave to receive)
    hspi.State = HAL_SPI_STATE_READY;

    uint8_t pData[10];
    uint16_t Size = 10;

    // Act: Call HAL_SPI_Receive
    HAL_StatusTypeDef rc = HAL_SPI_Receive(&hspi, pData, Size, 100);

    // Assert: The function should time out
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_TIMEOUT);
}

// Test Case: Verify that HAL_SPI_Receive_DMA transfers data correctly
void test_hal_spi_receive_dma_transfers_data(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    uint8_t pData[10];
    uint16_t Size = 10;

    // Data sent from slave to receive
    hspi.State = HAL_SPI_STATE_BUSY_RX;
    memset(hspi.RxMsgBuff, 2, Size);

    // Act: Call HAL_SPI_Receive
    HAL_StatusTypeDef rc = HAL_SPI_Receive_DMA(&hspi, pData, Size);

    // Assert: The data should be transferred correctly
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(pData, hspi.RxMsgBuff, Size);
}

// Test Case: Verify that HAL_SPI_Receive_DMA sets expected values in the SPI handle
void test_hal_spi_receive_dma_sets_values(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    uint8_t pData[10];
    uint16_t Size = 10;

    // Data sent from slave to receive
    hspi.State = HAL_SPI_STATE_BUSY_RX;
    memset(hspi.RxMsgBuff, 2, Size);

    // Act: Call HAL_SPI_Receive
    HAL_StatusTypeDef rc = HAL_SPI_Receive_DMA(&hspi, pData, Size);

    // Assert: Values in SPI handle should be set correctly according to current state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_READY);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
}

// Test Case: Verify that Mock_SPI_Slave_Transmit transfers data correctly
void test_mock_spi_slave_transmit_transfers_data(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    // Simulate 10-element message buffer requested by HAL_SPI_Receive
    hspi.State = HAL_SPI_STATE_READY;
    hspi.RxMsgSize = 10;

    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call Mock_SPI_Slave_Transmit
    HAL_StatusTypeDef rc = Mock_SPI_Slave_Transmit(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred correctly
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(hspi.RxMsgBuff, pData, Size);
}

// Test Case: Verify that Mock_SPI_Slave_Transmit sets the SPI handler to a busy state
void test_mock_spi_slave_transmit_sets_values(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_READY;

    // Simulate 10-element message buffer requested by HAL_SPI_Receive
    hspi.State = HAL_SPI_STATE_READY;
    hspi.RxMsgSize = 10;

    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call Mock_SPI_Slave_Transmit
    HAL_StatusTypeDef rc = Mock_SPI_Slave_Transmit(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: Values in SPI handle should be set correctly according to current state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_BUSY_RX);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
    assert_int_equal(hspi.RxMsgSize, Size);
}

// Test Case: Verify that Mock_SPI_Slave_Transmit times out correctly when SPI stays at improper state
void test_mock_spi_slave_transmit_timeout(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    // Transmit should time out if SPI is held at a busy state (Another transaction is ongoing)
    hspi.State = HAL_SPI_STATE_BUSY_RX;

    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Act: Call Mock_SPI_Slave_Transmit
    HAL_StatusTypeDef rc = Mock_SPI_Slave_Transmit(&hspi, pData, Size, 100);

    // Assert: The function should time out
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_TIMEOUT);
}

// Test Case: Verify that Mock_SPI_Slave_Transmit fails for a message size not promised by HAL_SPI_Receive
void test_mock_spi_slave_transmit_size_mismatch(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    // Transmit should fail if the size of the message to be sent is different from the one requested by the master
    hspi.State = HAL_SPI_STATE_READY;
    hspi.RxMsgSize = 10;

    uint8_t pData[5] = {0, 1, 2, 3, 4};
    uint16_t Size = 5;

    // Act: Call Mock_SPI_Slave_Transmit
    HAL_StatusTypeDef rc = Mock_SPI_Slave_Transmit(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The function should fail
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_SIZE_MISMATCH);
}

// Test Case: Verify that Mock_SPI_Slave_Receive transfers data correctly
void test_mock_spi_slave_receive_transfers_data(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    uint8_t pData[10];
    uint16_t Size = 10;

    // Data sent from master to receive
    hspi.State = HAL_SPI_STATE_BUSY_TX;
    hspi.TxMsgSize = 10;
    memset(hspi.TxMsgBuff, 2, Size);

    // Act: Call Mock_SPI_Slave_Receive
    HAL_StatusTypeDef rc = Mock_SPI_Slave_Receive(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred correctly
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(pData, hspi.TxMsgBuff, Size);
}

// Test Case: Verify that Mock_SPI_Slave_Receive sets expected values in the SPI handle
void test_mock_spi_slave_receive_sets_values(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    uint8_t pData[10];
    uint16_t Size = 10;

    // Data sent from master to receive
    hspi.State = HAL_SPI_STATE_BUSY_TX;
    hspi.TxMsgSize = 10;
    memset(hspi.TxMsgBuff, 2, Size);
    
    // Act: Call Mock_SPI_Slave_Receive
    HAL_StatusTypeDef rc = Mock_SPI_Slave_Receive(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: Values in SPI handle should be set correctly according to current state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_READY);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
}

// Test Case: Verify that Mock_SPI_Slave_Receive times out correctly when SPI stays at improper state
void test_mock_spi_slave_receive_timeout(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    // Receive should time out if SPI is held at a ready state (No data sent from master to receive)
    hspi.State = HAL_SPI_STATE_READY;

    uint8_t pData[10];
    uint16_t Size = 10;
    hspi.TxMsgSize = 10;

    // Act: Call Mock_SPI_Slave_Receive
    HAL_StatusTypeDef rc = Mock_SPI_Slave_Receive(&hspi, pData, Size, 100);

    // Assert: The function should time out
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_TIMEOUT);
}

// Test Case: Verify that Mock_SPI_Slave_Receive fails for a message size not promised by HAL_SPI_Transmit
void test_mock_spi_slave_receive_size_mismatch(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;

    // Receive should fail if the size of the message to be received is different from the one promised by the master
    hspi.State = HAL_SPI_STATE_BUSY_TX;
    hspi.TxMsgSize = 10;

    uint8_t pData[5];
    uint16_t Size = 5;

    // Act: Call Mock_SPI_Slave_Receive
    HAL_StatusTypeDef rc = Mock_SPI_Slave_Receive(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The function should fail
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_SIZE_MISMATCH);
}