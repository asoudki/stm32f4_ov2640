#ifndef TEST_HAL_MOCK_GPIO_H
#define TEST_HAL_MOCK_GPIO_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../../mocks/hal_mock_gpio.h"

// Declaration of test functions

// HAL_GPIO_Init Tests
void test_hal_mock_gpio_init_returns_ok(void **state);
void test_hal_mock_gpio_init_sets_regs(void **state);
void test_hal_mock_gpio_init_null_input(void **state);
void test_hal_mock_gpio_init_no_hal_init(void **state);

// HAL_GPIO_ReadPin Tests
void test_hal_mock_read_pin_reset_state(void **state);
void test_hal_mock_read_pin_set_state(void **state);
void test_hal_mock_read_pin_multiple(void **state);
void test_hal_mock_read_pin_invalid_pin(void **state);
void test_hal_mock_read_pin_null_input(void **state);
void test_hal_mock_read_pin_no_hal_init(void **state);

// HAL_GPIO_WritePin Tests
void test_hal_mock_write_pin_set_state(void **state);
void test_hal_mock_write_pin_reset_state(void **state);
void test_hal_mock_write_pin_multiple(void **state);
void test_hal_mock_write_pin_invalid_pin(void **state);
void test_hal_mock_write_pin_null_input(void **state);
void test_hal_mock_write_pin_no_hal_init(void **state);

#endif // TEST_HAL_MOCK_GPIO_H