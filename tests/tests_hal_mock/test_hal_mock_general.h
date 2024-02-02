#ifndef TEST_HAL_MOCK_GENERAL_H
#define TEST_HAL_MOCK_GENERAL_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../../mocks/hal_mock_general.h"

// Defines (number of tests, change as more are added)
#define NUM_HAL_MOCK_HAL_INIT_TESTS 2
#define NUM_HAL_MOCK_DELAY_TESTS 5

// Global test arrays
extern const struct CMUnitTest hal_mock_hal_init_tests[NUM_HAL_MOCK_HAL_INIT_TESTS];
extern const struct CMUnitTest hal_mock_delay_tests[NUM_HAL_MOCK_DELAY_TESTS];

// Declaration of test functions

// Running all tests
void run_hal_mock_general_tests(void);

// HAL_Init Tests
void test_hal_mock_hal_init_starts_at_zero(void **state);
void test_hal_mock_hal_init_changes_initialized(void **state);

// HAL_Delay Tests
void test_hal_mock_delay_zero(void **state);
void test_hal_mock_delay_short_duration(void **state);
void test_hal_mock_delay_long_duration(void **state);
void test_hal_mock_delay_consecutive(void **state);
void test_hal_mock_delay_no_hal_init(void **state);

#endif // TEST_HAL_MOCK_GENERAL_H
