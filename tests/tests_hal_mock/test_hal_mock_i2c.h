#ifndef TEST_HAL_MOCK_I2C_H
#define TEST_HAL_MOCK_I2C_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include <cmocka.h>

#include "../../mocks/hal_mock_i2c.h"

// Defines (number of tests, change as more are added)
#define NUM_COMMON_I2C_CHECKS_TESTS 3
#define NUM_COMMON_MASTER_TRANSACTION_CHECKS_TESTS 4
#define NUM_SET_I2C_MOCK_SLAVE_TESTS 1
#define NUM_HAL_I2C_INIT_TESTS 1
#define NUM_HAL_I2C_DEINIT_TESTS 1
#define NUM_HAL_I2C_MASTER_TRANSMIT_TESTS 2
#define NUM_HAL_I2C_MASTER_RECEIVE_TESTS 2

// Global test arrays
extern const struct CMUnitTest common_i2c_checks_tests[NUM_COMMON_I2C_CHECKS_TESTS];
extern const struct CMUnitTest common_master_transaction_checks_tests[NUM_COMMON_MASTER_TRANSACTION_CHECKS_TESTS];
extern const struct CMUnitTest set_i2c_mock_slave_tests[NUM_SET_I2C_MOCK_SLAVE_TESTS];
extern const struct CMUnitTest hal_i2c_init_tests[NUM_HAL_I2C_INIT_TESTS];
extern const struct CMUnitTest hal_i2c_deinit_tests[NUM_HAL_I2C_DEINIT_TESTS];
extern const struct CMUnitTest hal_i2c_master_transmit_tests[NUM_HAL_I2C_MASTER_TRANSMIT_TESTS];
extern const struct CMUnitTest hal_i2c_master_receive_tests[NUM_HAL_I2C_MASTER_RECEIVE_TESTS];

// Declaration of test functions

// Running all tests
void run_hal_mock_i2c_tests(void);

// common_i2c_checks Tests
void test_common_i2c_checks_returns_ok(void **state);
void test_common_i2c_checks_null_input(void **state);
void test_common_i2c_checks_no_hal_init(void **state);

// common_i2c_master_transaction_checks Tests
void test_common_i2c_master_transaction_checks_returns_ok(void **state);
void test_common_i2c_master_transaction_checks_null_pdata(void **state);
void test_common_i2c_master_transaction_checks_non_ready_state(void **state);
void test_common_i2c_master_transaction_checks_bad_slave(void **state);

// Set_I2C_Mock_Slave Tests
void test_set_i2c_mock_slave_sets_values(void **state);

// HAL_I2C_Init Tests
void test_hal_i2c_init_sets_values(void **state);

// HAL_I2C_DeInit Tests
void test_hal_i2c_deinit_sets_values(void **state);

// HAL_I2C_Master_Transmit Tests
void test_hal_i2c_master_transmit_transfers_data(void **state);
void test_hal_i2c_master_transmit_sets_state(void **state);

// HAL_I2C_Master_Receive Tests
void test_hal_i2c_master_receive_transfers_data(void **state);
void test_hal_i2c_master_receive_sets_state(void **state);

#endif // TEST_HAL_MOCK_I2C_H