#ifndef TEST_HAL_MOCK_GPIO_H
#define TEST_HAL_MOCK_GPIO_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../../mocks/hal_mock_gpio.h"

// Defines (number of tests, change as more are added)
#define NUM_HAL_MOCK_GPIO_INIT_TESTS 4
#define NUM_HAL_MOCK_READ_PIN_TESTS 6
#define NUM_HAL_MOCK_WRITE_PIN_TESTS 6

// Global test arrays
extern const struct CMUnitTest hal_mock_gpio_init_tests[NUM_HAL_MOCK_GPIO_INIT_TESTS];
extern const struct CMUnitTest hal_mock_read_pin_tests[NUM_HAL_MOCK_READ_PIN_TESTS];
extern const struct CMUnitTest hal_mock_write_pin_tests[NUM_HAL_MOCK_WRITE_PIN_TESTS];

// Declaration of test functions

// Running all tests
void run_hal_mock_gpio_tests(void);

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