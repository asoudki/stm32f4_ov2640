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
    cmocka_unit_test(test_common_spi_transaction_checks_non_ready_state)
};

// Set_SPI_Transmit_Mock_Slave Tests
const struct CMUnitTest set_spi_transmit_mock_slave_tests[NUM_SET_SPI_TRANSMIT_MOCK_SLAVE_TESTS] = {
    cmocka_unit_test(test_set_spi_transmit_mock_slave_sets_values)
};

// Set_SPI_Receive_Mock_Slave Tests
const struct CMUnitTest set_spi_receive_mock_slave_tests[NUM_SET_SPI_RECEIVE_MOCK_SLAVE_TESTS] = {
    cmocka_unit_test(test_set_spi_receive_mock_slave_sets_values)
};

// HAL_SPI_Init Tests
const struct CMUnitTest hal_mock_spi_init_tests[NUM_HAL_MOCK_INIT_TESTS] = {
    cmocka_unit_test(test_hal_spi_init_sets_state)
};

// HAL_SPI_DeInit Tests
const struct CMUnitTest hal_mock_spi_deinit_tests[NUM_HAL_MOCK_DEINIT_TESTS] = {
    cmocka_unit_test(test_hal_spi_deinit_sets_state)
};

// HAL_SPI_Transmit Tests
const struct CMUnitTest hal_mock_transmit_tests[NUM_HAL_MOCK_TRANSMIT_TESTS] = {
    cmocka_unit_test(test_hal_spi_transmit_transfers_correctly),
    cmocka_unit_test(test_hal_spi_transmit_correct_state),
    cmocka_unit_test(test_hal_spi_transmit_bad_slave)
};

// HAL_SPI_Transmit_DMA Tests
const struct CMUnitTest hal_mock_transmit_dma_tests[NUM_HAL_MOCK_TRANSMIT_DMA_TESTS] = {
    cmocka_unit_test(test_hal_spi_transmit_dma_transfers_correctly),
    cmocka_unit_test(test_hal_spi_transmit_dma_correct_state),
    cmocka_unit_test(test_hal_spi_transmit_dma_bad_slave)
};

// HAL_SPI_Receive Tests
const struct CMUnitTest hal_mock_receive_tests[NUM_HAL_MOCK_RECEIVE_TESTS] = {
    cmocka_unit_test(test_hal_spi_receive_transfers_correctly),
    cmocka_unit_test(test_hal_spi_receive_correct_state),
    cmocka_unit_test(test_hal_spi_receive_bad_slave)
};

// HAL_SPI_Receive_DMA Tests
const struct CMUnitTest hal_mock_receive_dma_tests[NUM_HAL_MOCK_RECEIVE_DMA_TESTS] = {
    cmocka_unit_test(test_hal_spi_receive_dma_transfers_correctly),
    cmocka_unit_test(test_hal_spi_receive_dma_correct_state),
    cmocka_unit_test(test_hal_spi_receive_dma_bad_slave)
};

void run_hal_mock_spi_tests(void) {
    int status = 0;
    
    status += cmocka_run_group_tests(common_spi_checks_tests, NULL, NULL);
    status += cmocka_run_group_tests(common_spi_transaction_checks_tests, NULL, NULL);
    status += cmocka_run_group_tests(set_spi_transmit_mock_slave_tests, NULL, NULL);
    status += cmocka_run_group_tests(set_spi_receive_mock_slave_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_spi_init_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_spi_deinit_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_transmit_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_transmit_dma_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_receive_tests, NULL, NULL);
    status += cmocka_run_group_tests(hal_mock_receive_dma_tests, NULL, NULL);

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
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_HAL_UNITIALIZED);
}

// Test Case: Verify that common_spi_transaction_checks passes for an appropriately configured SPI handle
void test_common_spi_transaction_checks_returns_ok(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10];
    uint16_t Size = 10;

    HAL_SPI_Init(&hspi);
    Set_SPI_Transmit_Mock_Slave(&hspi, pData, Size);

    // Act: Call a function that uses the private common_spi_transaction_checks
    HAL_StatusTypeDef rc = HAL_SPI_Transmit(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The function should succeed
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
}

// Test Case: Verify that common_spi_transaction_checks fails for a null pData
void test_common_spi_transaction_checks_null_pdata(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10];
    uint16_t Size = 10;

    HAL_SPI_Init(&hspi);
    Set_SPI_Transmit_Mock_Slave(&hspi, NULL, Size);

    // Act: Call a function that uses the private common_spi_transaction_checks, but with a null pData
    HAL_StatusTypeDef rc = HAL_SPI_Transmit(&hspi, NULL, Size, HAL_MAX_DELAY);

    // Assert: common_spi_transaction_checks should catch the null pData and return an error
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NULL_PARAM);
}

// Test Case: Verify that common_spi_transaction_checks fails for all non-ready states
void test_common_spi_transaction_checks_non_ready_state(void **state) {
    // Arrange: Initialize HAL, create uninitialized, busy and erronous SPI handles and prepare for transactions with them
    hal_initialized = 1;
    SPI_HandleTypeDef hspi1, hspi2, hspi3;
    uint8_t pData[10];
    uint16_t Size = 10;

    hspi1.State = HAL_SPI_STATE_RESET;
    hspi2.State = HAL_SPI_STATE_BUSY;
    hspi3.State = HAL_SPI_STATE_ERROR;

    Set_SPI_Transmit_Mock_Slave(&hspi1, pData, Size);
    Set_SPI_Transmit_Mock_Slave(&hspi2, pData, Size);
    Set_SPI_Transmit_Mock_Slave(&hspi3, pData, Size);

    // Act: Call a function that uses the private common_spi_transaction_checks
    HAL_StatusTypeDef rc1 = HAL_SPI_Transmit(&hspi1, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc2 = HAL_SPI_Transmit(&hspi2, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc3 = HAL_SPI_Transmit(&hspi3, pData, Size, HAL_MAX_DELAY);

    // Assert: common_spi_transaction_checks should catch the non-ready states and return corresponding errors
    assert_int_equal(rc1, HAL_ERROR);
    assert_int_equal(hspi1.ErrorCode, HAL_SPI_ERROR_UNITIALIZED);
    assert_int_equal(rc2, HAL_BUSY);
    assert_int_equal(hspi2.ErrorCode, HAL_SPI_ERROR_BUSY);
    assert_int_equal(rc3, HAL_ERROR);
    assert_int_equal(hspi3.ErrorCode, HAL_SPI_ERROR_FAILSTATE);
}

// Test Case: Verify that Set_SPI_Transmit_Mock_Slave adds a tx slave to the SPI handle
void test_set_spi_transmit_mock_slave_sets_values(void **state) {
    // Arrange: Initialize HAL and create SPI handle and a buffer to pass in
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10];
    uint16_t Size = 10;

    // Act: Call Set_SPI_Transmit_Mock_Slave with the buffer
    HAL_StatusTypeDef rc = Set_SPI_Transmit_Mock_Slave(&hspi, pData, Size);

    // Assert: The values corresponding to the slave transmitted to should be set in the SPI handle
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.pTxBuffPtr, pData);
    assert_int_equal(hspi.TxXferSize, Size);
}

// Test Case: Verify that Set_SPI_Receive_Mock_Slave adds a rx slave to the SPI handle
void test_set_spi_receive_mock_slave_sets_values(void **state) {
    // Arrange: Initialize HAL and create SPI handle and a buffer to pass in
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10];
    uint16_t Size = 10;

    // Act: Call Set_SPI_Receive_Mock_Slave with the buffer
    HAL_StatusTypeDef rc = Set_SPI_Receive_Mock_Slave(&hspi, pData, Size);

    // Assert: The values corresponding to the slave received from should be set in the SPI handle
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.pRxBuffPtr, pData);
    assert_int_equal(hspi.RxXferSize, Size);
}

// Test Case: Verify that HAL_SPI_Init sets the state of the SPI handle to ready
void test_hal_spi_init_sets_state(void **state) {
    // Arrange: Initialize HAL and create a SPI handle with values different from initialized
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_RESET;
    hspi.ErrorCode = 32;

    // Act: Call HAL_SPI_Init
    HAL_StatusTypeDef rc = HAL_SPI_Init(&hspi);

    // Assert: The SPI handle should be set to a ready state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_READY);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
}

// Test Case: Verify that HAL_SPI_DeInit sets the state of the SPI handle to reset
void test_hal_spi_deinit_sets_state(void **state) {
    // Arrange: Initialize HAL and create SPI handle with values different from reset
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    hspi.State = HAL_SPI_STATE_ERROR;
    hspi.ErrorCode = HAL_SPI_ERROR_BAD_SLAVE;

    // Act: Call HAL_SPI_DeInit
    HAL_StatusTypeDef rc = HAL_SPI_DeInit(&hspi);

    // Assert: The SPI handle should be set to a reset state
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_RESET);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
}

// Test Case: Verify that HAL_SPI_Transmit transfers data correctly
void test_hal_spi_transmit_transfers_correctly(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t Slave[10];
    uint16_t Size = 10;

    HAL_SPI_Init(&hspi);
    Set_SPI_Transmit_Mock_Slave(&hspi, Slave, Size);

    // Act: Call HAL_SPI_Transmit
    HAL_StatusTypeDef rc = HAL_SPI_Transmit(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred correctly
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(Slave, pData, Size);
}

// Test Case: Verify that HAL_SPI_Transmit sets the state of the SPI handle back to a ready state afterwards
void test_hal_spi_transmit_correct_state(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t Slave[10];
    uint16_t Size = 10;

    HAL_SPI_Init(&hspi);
    Set_SPI_Transmit_Mock_Slave(&hspi, Slave, Size);

    // Act: Call HAL_SPI_Transmit
    HAL_StatusTypeDef rc = HAL_SPI_Transmit(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The state of the SPI handle should be set back to ready
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_READY);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
}

// Test Case: Verify that HAL_SPI_Transmit returns an error for a bad slave
void test_hal_spi_transmit_bad_slave(void **state) {
    // Arrange: Initialize HAL, create SPI handles with improperly registered slaves
    hal_initialized = 1;
    SPI_HandleTypeDef hspi1, hspi2, hspi3;
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t Slave[10];
    uint16_t Size = 10;

    // Slave isn't set
    HAL_SPI_Init(&hspi1);

    // Slave is set but with null pData
    HAL_SPI_Init(&hspi2);
    Set_SPI_Transmit_Mock_Slave(&hspi2, NULL, Size);

    // Slave is set but with different Size
    HAL_SPI_Init(&hspi3);
    Set_SPI_Transmit_Mock_Slave(&hspi3, Slave, Size+1);


    // Act: Call HAL_SPI_Transmit without setting a mock slave
    HAL_StatusTypeDef rc1 = HAL_SPI_Transmit(&hspi1, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc2 = HAL_SPI_Transmit(&hspi2, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc3 = HAL_SPI_Transmit(&hspi3, pData, Size, HAL_MAX_DELAY);

    // Assert: HAL_SPI_Transmit should return corresponding errors for each combination of bad slave
    assert_int_equal(rc1, HAL_ERROR);
    assert_int_equal(hspi1.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
    assert_int_equal(rc2, HAL_ERROR);
    assert_int_equal(hspi2.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
    assert_int_equal(rc3, HAL_ERROR);
    assert_int_equal(hspi3.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
}

// Test Case: Verify that HAL_SPI_Transmit_DMA transfers data correctly
void test_hal_spi_transmit_dma_transfers_correctly(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t Slave[10];
    uint16_t Size = 10;

    HAL_SPI_Init(&hspi);
    Set_SPI_Transmit_Mock_Slave(&hspi, Slave, Size);

    // Act: Call HAL_SPI_Transmit_DMA
    HAL_StatusTypeDef rc = HAL_SPI_Transmit_DMA(&hspi, pData, Size);

    // Assert: The data should be transferred correctly
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(Slave, pData, Size);
}

// Test Case: Verify that HAL_SPI_Transmit_DMA sets the state of the SPI handle back to a ready state afterwards
void test_hal_spi_transmit_dma_correct_state(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t Slave[10];
    uint16_t Size = 10;

    HAL_SPI_Init(&hspi);
    Set_SPI_Transmit_Mock_Slave(&hspi, Slave, Size);

    // Act: Call HAL_SPI_Transmit_DMA
    HAL_StatusTypeDef rc = HAL_SPI_Transmit_DMA(&hspi, pData, Size);

    // Assert: The state of the SPI handle should be set back to ready
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_READY);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
}

// Test Case: Verify that HAL_SPI_Transmit_DMA returns an error for a bad slave
void test_hal_spi_transmit_dma_bad_slave(void **state) {
    // Arrange: Initialize HAL, create SPI handles with improperly registered slaves
    hal_initialized = 1;
    SPI_HandleTypeDef hspi1, hspi2, hspi3;
    uint8_t pData[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t Slave[10];
    uint16_t Size = 10;

    // Slave isn't set
    HAL_SPI_Init(&hspi1);

    // Slave is set but with null pData
    HAL_SPI_Init(&hspi2);
    Set_SPI_Transmit_Mock_Slave(&hspi2, NULL, Size);

    // Slave is set but with different Size
    HAL_SPI_Init(&hspi3);
    Set_SPI_Transmit_Mock_Slave(&hspi3, Slave, Size+1);

    // Act: Call HAL_SPI_Transmit_DMA without setting a mock slave
    HAL_StatusTypeDef rc1 = HAL_SPI_Transmit_DMA(&hspi1, pData, Size);
    HAL_StatusTypeDef rc2 = HAL_SPI_Transmit_DMA(&hspi2, pData, Size);
    HAL_StatusTypeDef rc3 = HAL_SPI_Transmit_DMA(&hspi3, pData, Size);

    // Assert: HAL_SPI_Transmit_DMA should return corresponding errors for each combination of bad slave
    assert_int_equal(rc1, HAL_ERROR);
    assert_int_equal(hspi1.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
    assert_int_equal(rc2, HAL_ERROR);
    assert_int_equal(hspi2.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
    assert_int_equal(rc3, HAL_ERROR);
    assert_int_equal(hspi3.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
}

// Test Case: Verify that HAL_SPI_Receive transfers data correctly
void test_hal_spi_receive_transfers_correctly(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10];
    uint8_t Slave[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    HAL_SPI_Init(&hspi);
    Set_SPI_Receive_Mock_Slave(&hspi, Slave, Size);

    // Act: Call HAL_SPI_Receive
    HAL_StatusTypeDef rc = HAL_SPI_Receive(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The data should be transferred correctly
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(pData, Slave, Size);
}

// Test Case: Verify that HAL_SPI_Receive sets the state of the SPI handle back to a ready state afterwards
void test_hal_spi_receive_correct_state(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10];
    uint8_t Slave[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    HAL_SPI_Init(&hspi);
    Set_SPI_Receive_Mock_Slave(&hspi, Slave, Size);

    // Act: Call HAL_SPI_Receive
    HAL_StatusTypeDef rc = HAL_SPI_Receive(&hspi, pData, Size, HAL_MAX_DELAY);

    // Assert: The state of the SPI handle should be set back to ready
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_READY);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
}

// Test Case: Verify that HAL_SPI_Receive returns an error for a bad slave
void test_hal_spi_receive_bad_slave(void **state) {
    // Arrange: Initialize HAL, create SPI handles with improperly registered slaves
    hal_initialized = 1;
    SPI_HandleTypeDef hspi1, hspi2, hspi3;
    uint8_t pData[10];
    uint8_t Slave[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Slave isn't set
    HAL_SPI_Init(&hspi1);

    // Slave is set but with null pData
    HAL_SPI_Init(&hspi2);
    Set_SPI_Receive_Mock_Slave(&hspi2, NULL, Size);

    // Slave is set but with different Size
    HAL_SPI_Init(&hspi3);
    Set_SPI_Receive_Mock_Slave(&hspi3, Slave, Size+1);

    // Act: Call HAL_SPI_Receive without setting a mock slave
    HAL_StatusTypeDef rc1 = HAL_SPI_Receive(&hspi1, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc2 = HAL_SPI_Receive(&hspi2, pData, Size, HAL_MAX_DELAY);
    HAL_StatusTypeDef rc3 = HAL_SPI_Receive(&hspi3, pData, Size, HAL_MAX_DELAY);

    // Assert: HAL_SPI_Receive should return corresponding errors for each combination of bad slave
    assert_int_equal(rc1, HAL_ERROR);
    assert_int_equal(hspi1.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
    assert_int_equal(rc2, HAL_ERROR);
    assert_int_equal(hspi2.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
    assert_int_equal(rc3, HAL_ERROR);
    assert_int_equal(hspi3.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
}

// Test Case: Verify that HAL_SPI_Receive_DMA transfers data correctly
void test_hal_spi_receive_dma_transfers_correctly(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10];
    uint8_t Slave[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    HAL_SPI_Init(&hspi);
    Set_SPI_Receive_Mock_Slave(&hspi, Slave, Size);

    // Act: Call HAL_SPI_Receive_DMA
    HAL_StatusTypeDef rc = HAL_SPI_Receive_DMA(&hspi, pData, Size);

    // Assert: The data should be transferred correctly
    assert_int_equal(rc, HAL_OK);
    assert_memory_equal(pData, Slave, Size);
}

// Test Case: Verify that HAL_SPI_Receive_DMA sets the state of the SPI handle back to a ready state afterwards
void test_hal_spi_receive_dma_correct_state(void **state) {
    // Arrange: Initialize HAL, create SPI handle and prepare for a transaction
    hal_initialized = 1;
    SPI_HandleTypeDef hspi;
    uint8_t pData[10];
    uint8_t Slave[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    HAL_SPI_Init(&hspi);
    Set_SPI_Receive_Mock_Slave(&hspi, Slave, Size);

    // Act: Call HAL_SPI_Receive_DMA
    HAL_StatusTypeDef rc = HAL_SPI_Receive_DMA(&hspi, pData, Size);

    // Assert: The state of the SPI handle should be set back to ready
    assert_int_equal(rc, HAL_OK);
    assert_int_equal(hspi.State, HAL_SPI_STATE_READY);
    assert_int_equal(hspi.ErrorCode, HAL_SPI_ERROR_NONE);
}

// Test Case: Verify that HAL_SPI_Receive_DMA returns an error for a bad slave
void test_hal_spi_receive_dma_bad_slave(void **state) {
    // Arrange: Initialize HAL, create SPI handles with improperly registered slaves
    hal_initialized = 1;
    SPI_HandleTypeDef hspi1, hspi2, hspi3;
    uint8_t pData[10];
    uint8_t Slave[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint16_t Size = 10;

    // Slave isn't set
    HAL_SPI_Init(&hspi1);

    // Slave is set but with null pData
    HAL_SPI_Init(&hspi2);
    Set_SPI_Receive_Mock_Slave(&hspi2, NULL, Size);

    // Slave is set but with different Size
    HAL_SPI_Init(&hspi3);
    Set_SPI_Receive_Mock_Slave(&hspi3, Slave, Size+1);

    // Act: Call HAL_SPI_Receive_DMA without setting a mock slave
    HAL_StatusTypeDef rc1 = HAL_SPI_Receive_DMA(&hspi1, pData, Size);
    HAL_StatusTypeDef rc2 = HAL_SPI_Receive_DMA(&hspi2, pData, Size);
    HAL_StatusTypeDef rc3 = HAL_SPI_Receive_DMA(&hspi3, pData, Size);

    // Assert: HAL_SPI_Receive_DMA should return corresponding errors for each combination of bad slave
    assert_int_equal(rc1, HAL_ERROR);
    assert_int_equal(hspi1.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
    assert_int_equal(rc2, HAL_ERROR);
    assert_int_equal(hspi2.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
    assert_int_equal(rc3, HAL_ERROR);
    assert_int_equal(hspi3.ErrorCode, HAL_SPI_ERROR_BAD_SLAVE);
}
