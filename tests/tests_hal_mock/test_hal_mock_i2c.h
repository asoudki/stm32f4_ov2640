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
#define NUM_COMMON_I2C_CHECKS_TESTS 5
#define NUM_COMMON_MASTER_TRANSACTION_CHECKS_TESTS 5
#define NUM_SET_I2C_MOCK_SLAVE_TESTS 3
#define NUM_HAL_MOCK_RESET_I2C_HANDLE_TESTS 7
#define NUM_HAL_MOCK_INIT_TESTS 8
#define NUM_HAL_MOCK_DEINIT_TESTS 8
#define NUM_HAL_MOCK_MASTER_TRANSMIT_TESTS 3
#define NUM_HAL_MOCK_MASTER_RECEIVE_TESTS 3

// Global test arrays
extern const struct CMUnitTest common_i2c_checks_tests[NUM_COMMON_I2C_CHECKS_TESTS];
extern const struct CMUnitTest common_master_transaction_checks_tests[NUM_COMMON_MASTER_TRANSACTION_CHECKS_TESTS];
extern const struct CMUnitTest set_i2c_mock_slave_tests[NUM_SET_I2C_MOCK_SLAVE_TESTS];
extern const struct CMUnitTest hal_mock_reset_i2c_handle_tests[NUM_HAL_MOCK_RESET_I2C_HANDLE_TESTS];
extern const struct CMUnitTest hal_mock_i2c_init_tests[NUM_HAL_MOCK_INIT_TESTS];
extern const struct CMUnitTest hal_mock_i2c_deinit_tests[NUM_HAL_MOCK_DEINIT_TESTS];
extern const struct CMUnitTest hal_mock_master_transmit_tests[NUM_HAL_MOCK_MASTER_TRANSMIT_TESTS];
extern const struct CMUnitTest hal_mock_master_receive_tests[NUM_HAL_MOCK_MASTER_RECEIVE_TESTS];
extern const struct CMUnitTest hal_mock_i2c_master_transmit_tests[NUM_HAL_MOCK_MASTER_TRANSMIT_TESTS];
extern const struct CMUnitTest hal_mock_i2c_master_receive_tests[NUM_HAL_MOCK_MASTER_RECEIVE_TESTS];

// Declaration of test functions

// Running all tests
void run_hal_mock_i2c_tests(void);

// common_i2c_checks Tests
void test_common_i2c_checks_returns_ok(void **state);
void test_common_i2c_checks_null_input(void **state);
void test_common_i2c_checks_no_hal_init(void **state);
void test_common_i2c_checks_invalid_instance(void **state);
void test_common_i2c_checks_handle_busy(void **state);

// common_i2c_master_transaction_checks Tests
void test_common_i2c_master_transaction_checks_returns_ok(void **state);
void test_common_i2c_master_transaction_checks_null_pdata(void **state);
void test_common_i2c_master_transaction_checks_invalid_address(void **state);
void test_common_i2c_master_transaction_checks_busy(void **state);
void test_common_i2c_master_transaction_checks_injected(void **state);

// Set_I2C_Slave_For_Current_Transaction Tests
void test_set_i2c_mock_slave_sets_values(void **state);
void test_set_i2c_mock_slave_null_input(void **state);
void test_set_i2c_mock_slave_invalid_size(void **state);

// Reset_I2C_Handle Tests
void test_hal_mock_reset_i2c_handle_changes_state(void **state);
void test_hal_mock_reset_i2c_handle_changes_mode(void **state);
void test_hal_mock_reset_i2c_handle_from_error(void **state);
void test_hal_mock_reset_i2c_handle_left_unlocked(void **state);
void test_hal_mock_reset_i2c_handle_resets_instance(void **state);
void test_hal_mock_reset_i2c_handle_keeps_init(void **state);
void test_hal_mock_reset_i2c_handle_repeat_reset(void **state);

// HAL_I2C_Init Tests
void test_hal_mock_i2c_init_changes_state(void **state);
void test_hal_mock_i2c_init_changes_mode(void **state);
void test_hal_mock_i2c_init_from_error(void **state);
void test_hal_mock_i2c_init_left_unlocked(void **state);
void test_hal_mock_i2c_init_keeps_init(void **state);
void test_hal_mock_i2c_init_resets_instance(void **state);
void test_hal_mock_i2c_init_already_initialized(void **state);
void test_hal_mock_i2c_init_repeat_init(void **state);

// HAL_I2C_DeInit Tests
void test_hal_mock_i2c_deinit_changes_state(void **state);
void test_hal_mock_i2c_deinit_changes_mode(void **state);
void test_hal_mock_i2c_deinit_from_error(void **state);
void test_hal_mock_i2c_deinit_left_unlocked(void **state);
void test_hal_mock_i2c_deinit_keeps_init(void **state);
void test_hal_mock_i2c_deinit_resets_instance(void **state);
void test_hal_mock_i2c_deinit_busy_peripheral(void **state);
void test_hal_mock_i2c_deinit_repeat_deinit(void **state);

// HAL_I2C_Master_Transmit Tests
void test_hal_mock_i2c_master_transmit_transfers_data(void **state);
void test_hal_mock_i2c_master_transmit_sets_transfer_fields(void **state);
void test_hal_mock_i2c_master_transmit_resets_state(void **state);

// HAL_I2C_Master_Receive Tests
void test_hal_mock_i2c_master_receive_transfers_data(void **state);
void test_hal_mock_i2c_master_receive_sets_transfer_fields(void **state);
void test_hal_mock_i2c_master_receive_resets_state(void **state);

#endif // TEST_HAL_MOCK_I2C_H