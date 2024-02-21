#ifndef TEST_HAL_MOCK_SPI_H
#define TEST_HAL_MOCK_SPI_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include <cmocka.h>

#include "../../mocks/hal_mock_spi.h"

// Defines (number of tests, change as more are added)
#define NUM_COMMON_SPI_CHECKS_TESTS 3
#define NUM_COMMON_SPI_TRANSACTION_CHECKS_TESTS 4
#define NUM_HAL_MOCK_INIT_TESTS 1
#define NUM_HAL_MOCK_DEINIT_TESTS 1
#define NUM_HAL_MOCK_TRANSMIT_TESTS 2
#define NUM_HAL_MOCK_TRANSMIT_DMA_TESTS 2
#define NUM_HAL_MOCK_RECEIVE_TESTS 2
#define NUM_HAL_MOCK_RECEIVE_DMA_TESTS 2

// Global test arrays
extern const struct CMUnitTest common_spi_checks_tests[NUM_COMMON_SPI_CHECKS_TESTS];
extern const struct CMUnitTest common_spi_transaction_checks_tests[NUM_COMMON_SPI_TRANSACTION_CHECKS_TESTS];
extern const struct CMUnitTest hal_mock_spi_init_tests[NUM_HAL_MOCK_INIT_TESTS];
extern const struct CMUnitTest hal_mock_spi_deinit_tests[NUM_HAL_MOCK_DEINIT_TESTS];
extern const struct CMUnitTest hal_mock_transmit_tests[NUM_HAL_MOCK_TRANSMIT_TESTS];
extern const struct CMUnitTest hal_mock_transmit_dma_tests[NUM_HAL_MOCK_TRANSMIT_DMA_TESTS];
extern const struct CMUnitTest hal_mock_receive_tests[NUM_HAL_MOCK_RECEIVE_TESTS];
extern const struct CMUnitTest hal_mock_receive_dma_tests[NUM_HAL_MOCK_RECEIVE_DMA_TESTS];

// Declaration of test functions

// Running all tests
void run_hal_mock_spi_tests(void);

// common_spi_checks Tests
void test_common_spi_checks_returns_ok(void **state);
void test_common_spi_checks_null_input(void **state);
void test_common_spi_checks_no_hal_init(void **state);

// common_spi_transaction_checks Tests
void test_common_spi_transaction_checks_returns_ok(void **state);
void test_common_spi_transaction_checks_null_pdata(void **state);
void test_common_spi_transaction_checks_non_ready_state(void **state);
void test_common_spi_master_transaction_checks_msg_too_big(void **state);

// HAL_SPI_Init Tests
void test_hal_spi_init_sets_values(void **state);

// HAL_SPI_DeInit Tests
void test_hal_spi_deinit_sets_values(void **state);

// HAL_SPI_Transmit Tests
void test_hal_spi_transmit_transfers_data(void **state);
void test_hal_spi_transmit_sets_state(void **state);

// HAL_SPI_Transmit_DMA Tests
void test_hal_spi_transmit_dma_transfers_data(void **state);
void test_hal_spi_transmit_dma_sets_state(void **state);

// HAL_SPI_Receive Tests
void test_hal_spi_receive_transfers_data(void **state);
void test_hal_spi_receive_sets_state(void **state);

// HAL_SPI_Receive_DMA Tests
void test_hal_spi_receive_dma_transfers_data(void **state);
void test_hal_spi_receive_dma_sets_state(void **state);

#endif // TEST_HAL_MOCK_SPI_H