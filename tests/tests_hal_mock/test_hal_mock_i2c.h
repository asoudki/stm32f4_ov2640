#ifndef TEST_HAL_MOCK_I2C_H
#define TEST_HAL_MOCK_I2C_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include <cmocka.h>

#include "../../mocks/hal_mock_i2c.h"

// Declaration of test functions

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