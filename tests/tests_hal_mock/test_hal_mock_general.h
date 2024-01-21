#ifndef TEST_HAL_MOCK_GENERAL_H
#define TEST_HAL_MOCK_GENERAL_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../../mocks/hal_mock_general.h"

// Declaration of test functions

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
