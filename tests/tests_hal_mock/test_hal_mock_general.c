#include "test_hal_mock_general.h"

// HAL_Init Tests
const struct CMUnitTest hal_mock_hal_init_tests[NUM_HAL_MOCK_HAL_INIT_TESTS] = {
    cmocka_unit_test(test_hal_mock_hal_init_starts_at_zero),
    cmocka_unit_test(test_hal_mock_hal_init_changes_initialized),
};

// HAL_Delay Tests
const struct CMUnitTest hal_mock_delay_tests[NUM_HAL_MOCK_DELAY_TESTS] = {
    cmocka_unit_test(test_hal_mock_delay_zero),
    cmocka_unit_test(test_hal_mock_delay_short_duration),
    cmocka_unit_test(test_hal_mock_delay_long_duration),
    cmocka_unit_test(test_hal_mock_delay_consecutive),
    cmocka_unit_test(test_hal_mock_delay_no_hal_init),
};

// Running all tests
void run_hal_mock_general_tests(void) {
    const struct CMUnitTest hal_mock_general_tests[] = {
        // HAL_Init Tests
        cmocka_unit_test(test_hal_mock_hal_init_starts_at_zero),
        cmocka_unit_test(test_hal_mock_hal_init_changes_initialized),

        // HAL_Delay Tests
        cmocka_unit_test(test_hal_mock_delay_zero),
        cmocka_unit_test(test_hal_mock_delay_short_duration),
        cmocka_unit_test(test_hal_mock_delay_long_duration),
        cmocka_unit_test(test_hal_mock_delay_consecutive),
        cmocka_unit_test(test_hal_mock_delay_no_hal_init),
    };

    cmocka_run_group_tests(hal_mock_general_tests, NULL, NULL);
}

// Test case: Verify that hal_initialized starts at 0
void test_hal_mock_hal_init_starts_at_zero(void **state) {
    // Arrange: No specific arrangement needed, as the default value of hal_initialized is 0

    // Act: No specific action needed for this test case

    // Assert: Verify that hal_initialized is 0
    assert_int_equal(hal_initialized, 0);
}

// Test case: Verify that calling HAL_Init changes hal_initialized from 0 to 1
void test_hal_mock_hal_init_changes_initialized(void **state) {
    // Arrange: Set hal_initialized to 0 to simulate uninitialized state
    hal_initialized = 0;

    // Act: Initialize HAL
    HAL_Init();

    // Assert: Verify that hal_initialized is changed to 1
    assert_int_equal(hal_initialized, 1);
}

// Test Case: Delay for 0 milliseconds
void test_hal_mock_delay_zero(void **state) {
    // Arrange: Record the current time
    uint32_t t_start = hal_current_time;

    // Act: Initialize HAL and perform delay for 0 milliseconds
    HAL_Init();
    HAL_Delay(0);

    // Assert: Verify that time remains unchanged
    assert_int_equal(hal_current_time, t_start);
}

// Test Case: Delay for a short duration
void test_hal_mock_delay_short_duration(void **state) {
    // Arrange: Record the current time
    uint32_t t_start = hal_current_time;

    // Act: Initialize HAL and perform delay for 10 milliseconds
    HAL_Init();
    HAL_Delay(10);

    // Assert: Verify that time has advanced by 10 milliseconds
    assert_int_equal(hal_current_time, t_start + 10);
}

// Test Case: Delay for a longer duration
void test_hal_mock_delay_long_duration(void **state) {
    // Arrange: Record the current time
    uint32_t t_start = hal_current_time;

    // Act: Initialize HAL and perform delay for 1000 milliseconds
    HAL_Init();
    HAL_Delay(1000);

    // Assert: Verify that time has advanced by 1000 milliseconds
    assert_int_equal(hal_current_time, t_start + 1000);
}

// Test Case: Call delay consecutively
void test_hal_mock_delay_consecutive(void **state) {
    // Arrange: Record the current time
    uint32_t t_start = hal_current_time;

    // Act: Initialize HAL and perform two consecutive delays for 10 milliseconds each
    HAL_Init();
    HAL_Delay(10);
    HAL_Delay(10);

    // Assert: Verify that time has advanced by 20 milliseconds
    assert_int_equal(hal_current_time, t_start + 20);
}

// Test Case: Call delay before initializing HAL (No delay should happen)
void test_hal_mock_delay_no_hal_init(void **state) {
    // Arrange: Set hal_initialized to 0 to simulate uninitialized state, Record the current time
    hal_initialized = 0;
    uint32_t t_start = hal_current_time;

    // Act: Attempt to perform a delay without initializing HAL
    HAL_Delay(100);

    // Assert: Verify that time remains unchanged
    assert_int_equal(hal_current_time, t_start);
}
