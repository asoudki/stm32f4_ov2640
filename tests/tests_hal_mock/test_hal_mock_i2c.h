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
#define NUM_HAL_MOCK_RESET_I2C_HANDLE_TESTS 8
#define NUM_HAL_MOCK_INIT_TESTS 9
#define NUM_HAL_MOCK_DEINIT_TESTS 9
#define NUM_HAL_MOCK_MASTER_TRANSMIT_TESTS 0
#define NUM_HAL_MOCK_MASTER_RECEIVE_TESTS 0
#define NUM_HAL_MOCK_SLAVE_TRANSMIT_TESTS 0
#define NUM_HAL_MOCK_SLAVE_RECEIVE_TESTS 0

// Global test arrays
extern const struct CMUnitTest common_i2c_checks_tests[NUM_COMMON_I2C_CHECKS_TESTS];
extern const struct CMUnitTest hal_mock_reset_i2c_handle_tests[NUM_HAL_MOCK_RESET_I2C_HANDLE_TESTS];
extern const struct CMUnitTest hal_mock_i2c_init_tests[NUM_HAL_MOCK_INIT_TESTS];
extern const struct CMUnitTest hal_mock_i2c_deinit_tests[NUM_HAL_MOCK_DEINIT_TESTS];
extern const struct CMUnitTest hal_mock_master_transmit_tests[NUM_HAL_MOCK_MASTER_TRANSMIT_TESTS];
extern const struct CMUnitTest hal_mock_master_receive_tests[NUM_HAL_MOCK_MASTER_RECEIVE_TESTS];
extern const struct CMUnitTest hal_mock_slave_transmit_tests[NUM_HAL_MOCK_SLAVE_TRANSMIT_TESTS];
extern const struct CMUnitTest hal_mock_slave_receive_tests[NUM_HAL_MOCK_SLAVE_RECEIVE_TESTS];

// Declaration of test functions

// Running all tests
void run_hal_mock_i2c_tests(void);

// common_i2c_checks Tests
void test_common_i2c_checks_returns_ok(void **state);
void test_common_i2c_checks_null_input(void **state);
void test_common_i2c_checks_no_hal_init(void **state);
void test_common_i2c_checks_invalid_instance(void **state);
void test_common_i2c_checks_handle_busy(void **state);

// Reset_I2C_Handle Tests
void test_hal_mock_reset_i2c_handle_changes_state(void **state);
void test_hal_mock_reset_i2c_handle_changes_mode(void **state);
void test_hal_mock_reset_i2c_handle_from_error(void **state);
void test_hal_mock_reset_i2c_handle_left_unlocked(void **state);
void test_hal_mock_reset_i2c_handle_clears_buffer(void **state);
void test_hal_mock_reset_i2c_handle_resets_instance(void **state);
void test_hal_mock_reset_i2c_handle_keeps_init(void **state);
void test_hal_mock_reset_i2c_handle_repeat_reset(void **state);

// HAL_I2C_Init Tests
void test_hal_mock_i2c_init_changes_state(void **state);
void test_hal_mock_i2c_init_changes_mode(void **state);
void test_hal_mock_i2c_init_from_error(void **state);
void test_hal_mock_i2c_init_left_unlocked(void **state);
void test_hal_mock_i2c_init_clears_buffer(void **state);
void test_hal_mock_i2c_init_keeps_init(void **state);
void test_hal_mock_i2c_init_resets_instance(void **state);
void test_hal_mock_i2c_init_already_initialized(void **state);
void test_hal_mock_i2c_init_repeat_init(void **state);

// HAL_I2C_DeInit Tests
void test_hal_mock_i2c_deinit_changes_state(void **state);
void test_hal_mock_i2c_deinit_changes_mode(void **state);
void test_hal_mock_i2c_deinit_from_error(void **state);
void test_hal_mock_i2c_deinit_left_unlocked(void **state);
void test_hal_mock_i2c_deinit_clears_buffer(void **state);
void test_hal_mock_i2c_deinit_keeps_init(void **state);
void test_hal_mock_i2c_deinit_resets_instance(void **state);
void test_hal_mock_i2c_deinit_busy_peripheral(void **state);
void test_hal_mock_i2c_deinit_repeat_deinit(void **state);

#endif // TEST_HAL_MOCK_I2C_H