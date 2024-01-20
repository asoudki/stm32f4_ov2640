#ifndef TEST_HAL_MOCK_GENERAL_H
#define TEST_HAL_MOCK_GENERAL_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../../mocks/hal_mock_general.h"

// Test case: Verify that hal_initialized starts at 0
void test_hal_mock_hal_init_starts_at_zero(void **state);

// Test case: Verify that calling HAL_Init changes hal_initialized from 0 to 1
void test_hal_mock_hal_init_changes_initialized(void **state);

// Test Case: Delay for 0 milliseconds
void test_hal_mock_delay_zero(void **state);

// Test Case: Delay for a short duration
void test_hal_mock_delay_short_duration(void **state);

// Test Case: Delay for a longer duration
void test_hal_mock_delay_long_duration(void **state);

// Test Case: Call delay consecutively
void test_hal_mock_delay_consecutive(void **state);

// Test Case: Call delay before initializing HAL (No delay should happen)
void test_hal_mock_delay_no_hal_init(void **state);

#endif // TEST_HAL_MOCK_GENERAL_H
