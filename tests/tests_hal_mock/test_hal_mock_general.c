#include "test_hal_mock_general.h"

// Test case: Verify that hal_initialized starts at 0
void test_hal_mock_hal_init_starts_at_zero(void **state) {
    // Unused parameter
    (void)state;

    // Arrange
    // hal_initialized should be 0 by default

    // Act
    // No action needed for this test case

    // Assert
    assert_int_equal(hal_initialized, 0);
}

// Test case: Verify that calling HAL_Init changes hal_initialized from 0 to 1
void test_hal_mock_hal_init_changes_initialized(void **state) {
    // Unused parameter
    (void)state;

    // Arrange
    hal_initialized = 0;

    // Act
    HAL_Init();

    // Assert
    assert_int_equal(hal_initialized, 1);
}

// Test Case: Delay for 0 milliseconds
void test_hal_mock_delay_zero(void **state) {
    // Unused parameter
    (void)state;

    // Arrange
    uint32_t t_start = hal_current_time;

    // Act
    HAL_Init();
    HAL_Delay(0);

    // Assert
    assert_int_equal(hal_current_time, t_start);
}

// Test Case: Delay for a short duration
void test_hal_mock_delay_short_duration(void **state) {
    // Unused parameter
    (void)state;

    // Arrange
    uint32_t t_start = hal_current_time;

    // Act
    HAL_Init();
    HAL_Delay(10);

    // Assert
    assert_int_equal(hal_current_time, t_start + 10);
}

// Test Case: Delay for a longer duration
void test_hal_mock_delay_long_duration(void **state) {
    // Unused parameter
    (void)state;

    // Arrange
    uint32_t t_start = hal_current_time;

    // Act
    HAL_Init();
    HAL_Delay(1000);

    // Assert
    assert_int_equal(hal_current_time, t_start + 1000);
}

// Test Case: Call delay consecutively
void test_hal_mock_delay_consecutive(void **state) {
    // Unused parameter
    (void)state;

    // Arrange
    uint32_t t_start = hal_current_time;

    // Act
    HAL_Init();
    HAL_Delay(10);
    HAL_Delay(10);

    // Assert
    assert_int_equal(hal_current_time, t_start + 20);
}

// Test Case: Call delay before initializing HAL (No delay should happen)
void test_hal_mock_delay_no_hal_init(void **state) {
    // Unused parameter
    (void)state;

    // Arrange
    hal_initialized = 0;
    uint32_t t_start = hal_current_time;

    // Act
    HAL_Delay(100);

    // Assert
    assert_int_equal(hal_current_time, t_start);
}