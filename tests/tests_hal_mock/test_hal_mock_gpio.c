#include "test_hal_mock_gpio.h"

// Required structs for GPIO
GPIO_TypeDef GPIO_Port;
GPIO_InitTypeDef GPIO_InitStruct;

// Test case: Verify that GPIO initialization returns HAL_OK when HAL is initialized first
void test_hal_mock_gpio_init_returns_ok(void **state) {
    // Arrange: Initialize the Hardware Abstraction Layer (HAL)
    hal_initialized = 1;

    // Act: Attempt to initialize GPIO
    HAL_StatusTypeDef rc = HAL_GPIO_Init(&GPIO_Port, &GPIO_InitStruct);

    // Assert: Ensure GPIO initialization returns HAL_OK
    assert_int_equal(rc, HAL_OK);
}

// Test case: Verify that GPIO initialization sets the expected initial values for IDR, ODR, and BSRR
void test_hal_mock_gpio_init_sets_regs(void **state) {
    // Arrange: Initialize HAL and set initial values for GPIO registers
    hal_initialized = 1;
    GPIO_Port.IDR = 1;
    GPIO_Port.ODR = 1;
    GPIO_Port.BSRR = 1;

    // Act: Initialize GPIO
    HAL_GPIO_Init(&GPIO_Port, &GPIO_InitStruct);

    // Assert: Verify GPIO registers are set to their expected initial values
    assert_int_equal(GPIO_Port.IDR, 0);
    assert_int_equal(GPIO_Port.ODR, 0);
    assert_int_equal(GPIO_Port.BSRR, 0);
}

// Test case: Verify behavior for null GPIO_TypeDef or GPIO_InitTypeDef in HAL_GPIO_Init
void test_hal_mock_gpio_init_null_input(void **state) {
    // Arrange: Initialize HAL
    hal_initialized = 1;

    // Act & Assert: Test with null GPIO_TypeDef
    assert_int_equal(HAL_GPIO_Init(NULL, &GPIO_InitStruct), HAL_ERROR);

    // Act & Assert: Test with null GPIO_InitTypeDef
    assert_int_equal(HAL_GPIO_Init(&GPIO_Port, NULL), HAL_ERROR);
}

// Test case: Attempt to initialize GPIO before initializing HAL (Should return HAL_ERROR and IDR/ODR/BSRR are unchanged)
void test_hal_mock_gpio_init_no_hal_init(void **state) {
    // Arrange: Set HAL as uninitialized and set initial values for GPIO registers
    hal_initialized = 0;
    GPIO_Port.IDR = 1;
    GPIO_Port.ODR = 1;
    GPIO_Port.BSRR = 1;

    // Act: Attempt to initialize GPIO
    HAL_StatusTypeDef rc = HAL_GPIO_Init(&GPIO_Port, &GPIO_InitStruct);

    // Assert: Ensure GPIO initialization returns HAL_ERROR and that IDR/ODR/BSRR remain unchanged
    assert_int_equal(rc, HAL_ERROR);
    assert_int_equal(GPIO_Port.IDR, 1);
    assert_int_equal(GPIO_Port.ODR, 1);
    assert_int_equal(GPIO_Port.BSRR, 1);
}

// Test case: Verify correct reads for a pin in the reset state
void test_hal_mock_read_pin_reset_state(void **state) {
    // Arrange: Set a specific GPIO pin to the reset state in the mock
    hal_initialized = 1;
    HAL_GPIO_Init(&GPIO_Port, &GPIO_InitStruct);

    // Act: Read the state of the specified GPIO pin
    GPIO_PinState result = HAL_GPIO_ReadPin(&GPIO_Port, GPIO_PIN_5);

    // Assert: Verify that the correct pin state (GPIO_PIN_RESET) is returned
    assert_int_equal(result, GPIO_PIN_RESET);
}

// Test case: Verify correct reads for a pin in the set state
void test_hal_mock_read_pin_set_state(void **state) {
    // Arrange: Set a specific GPIO pin to the set state in the mock
    hal_initialized = 1;
    HAL_GPIO_Init(&GPIO_Port, &GPIO_InitStruct);
    GPIO_Port.IDR |= GPIO_PIN_5;

    // Act: Read the state of the specified GPIO pin
    GPIO_PinState result = HAL_GPIO_ReadPin(&GPIO_Port, GPIO_PIN_5);

    // Assert: Verify that the correct pin state (GPIO_PIN_SET) is returned
    assert_int_equal(result, GPIO_PIN_SET);
}

// Test case: Verify reads for multiple GPIO pins in different states simultaneously
void test_hal_mock_read_pin_multiple(void **state) {
    // Arrange: Set various GPIO pins to different states in the mock
    hal_initialized = 1;
    HAL_GPIO_Init(&GPIO_Port, &GPIO_InitStruct);
    GPIO_Port.IDR |= GPIO_PIN_0;
    GPIO_Port.IDR |= GPIO_PIN_2;
    GPIO_Port.IDR |= GPIO_PIN_7;

    // Act: Read the state of multiple GPIO pins
    GPIO_PinState result1 = HAL_GPIO_ReadPin(&GPIO_Port, GPIO_PIN_0);
    GPIO_PinState result2 = HAL_GPIO_ReadPin(&GPIO_Port, GPIO_PIN_2);
    GPIO_PinState result3 = HAL_GPIO_ReadPin(&GPIO_Port, GPIO_PIN_7);

    // Assert: Verify the correctness of each pin state (All should be GPIO_PIN_SET)
    assert_int_equal(result1, GPIO_PIN_SET);
    assert_int_equal(result2, GPIO_PIN_SET);
    assert_int_equal(result3, GPIO_PIN_SET);
}

// Test case: Verify reads for an invalid GPIO pin
void test_hal_mock_read_pin_invalid_pin(void **state) {
    // Arrange: Initialize HAL and set initial values for GPIO registers
    hal_initialized = 1;
    GPIO_Port.IDR = GPIO_PIN_All;
    GPIO_Port.ODR = GPIO_PIN_All;
    GPIO_Port.BSRR = GPIO_PIN_0;
    uint16_t invalid_pin = 0x0000;

    // Act: Read the state of an invalid GPIO pin
    GPIO_PinState result = HAL_GPIO_ReadPin(&GPIO_Port, invalid_pin);

    // Assert: Verify the function returns the default pin state
    assert_int_equal(result, GPIO_PIN_RESET);
}

// Test case: Verify behavior for null GPIO_TypeDef in HAL_GPIO_ReadPin
void test_hal_mock_read_pin_null_input(void **state) {
    // Arrange: Initialize HAL and set initial values for GPIO registers
    hal_initialized = 1;
    GPIO_Port.IDR = GPIO_PIN_All;
    GPIO_Port.ODR = GPIO_PIN_All;
    GPIO_Port.BSRR = GPIO_PIN_0;

    // Act: Attempt to read a pin with null GPIO_TypeDef
    GPIO_PinState result = HAL_GPIO_ReadPin(NULL, GPIO_PIN_5);

    // Assert: Read should return 0 even though the pin is 1 because null GPIO_TypeDef was provided
    assert_int_equal(result, GPIO_PIN_RESET);
}

// Test case: Verify behavior when reading a pin with uninitialized HAL
void test_hal_mock_read_pin_no_hal_init(void **state) {
    // Arrange: Set HAL as uninitialized and set initial values for GPIO registers
    hal_initialized = 0;
    GPIO_Port.IDR = GPIO_PIN_All;
    GPIO_Port.ODR = GPIO_PIN_All;
    GPIO_Port.BSRR = GPIO_PIN_0;

    // Act: Read the state of a GPIO pin
    GPIO_PinState result = HAL_GPIO_ReadPin(&GPIO_Port, GPIO_PIN_5);

    // Assert: Read should return 0 even though the pin is 1 because HAL is not initialized
    assert_int_equal(result, GPIO_PIN_RESET);
}

// Test case: Set pin state from reset to set
void test_hal_mock_write_pin_set_state(void **state) {
    // Arrange: Initialize HAL and set the pin state to reset
    hal_initialized = 1;
    GPIO_Port.IDR = 0;            // Initial state: reset (Assumed)
    GPIO_Port.ODR = 0;            // Initial state: reset (Assumed)
    GPIO_Port.BSRR = 0;           // Initial state: reset (Assumed)

    // Act: Set the pin state to set
    HAL_GPIO_WritePin(&GPIO_Port, GPIO_PIN_5, GPIO_PIN_SET);

    // Assert: Verify that the pin state is set in IDR, ODR, and BSRR
    assert_int_equal(GPIO_Port.IDR, GPIO_PIN_5);      // Check IDR after writing to ODR
    assert_int_equal(GPIO_Port.ODR, GPIO_PIN_5);      // Check ODR after writing to ODR
    assert_int_equal(GPIO_Port.BSRR, GPIO_PIN_5);     // Check BSRR after writing to ODR
}

// Test case: Set pin state from set to reset
void test_hal_mock_write_pin_reset_state(void **state) {
    // Arrange: Initialize HAL and set the pin state to set
    hal_initialized = 1;
    GPIO_Port.IDR = GPIO_PIN_5;   // Initial state: set (Assumed)
    GPIO_Port.ODR = GPIO_PIN_5;   // Initial state: set (Assumed)
    GPIO_Port.BSRR = GPIO_PIN_5;  // Initial state: set (Assumed)

    // Act: Set the pin state to reset
    HAL_GPIO_WritePin(&GPIO_Port, GPIO_PIN_5, GPIO_PIN_RESET);

    // Assert: Verify that the pin state is reset in IDR, ODR, and BSRR
    assert_int_equal(GPIO_Port.IDR, GPIO_PIN_RESET);                // Check IDR after writing to ODR
    assert_int_equal(GPIO_Port.ODR, GPIO_PIN_RESET);                // Check ODR after writing to ODR
    assert_int_equal(GPIO_Port.BSRR, (uint32_t)GPIO_PIN_5 << 16U);  // Check BSRR after writing to ODR
}


// Test case: Write to multiple pins in a GPIO port
void test_hal_mock_write_pin_multiple(void **state) {
    // Arrange: Initialize HAL and set the GPIO port to reset
    hal_initialized = 1;
    GPIO_Port.IDR = 0;            // Initial state: reset
    GPIO_Port.ODR = 0;            // Initial state: reset
    GPIO_Port.BSRR = 0;           // Initial state: reset

    // Act: Write to multiple pins
    HAL_GPIO_WritePin(&GPIO_Port, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin(&GPIO_Port, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(&GPIO_Port, GPIO_PIN_7, GPIO_PIN_SET);

    // Assert: Verify the expected states after each write
    // Pin 0: Set, Pin 2: Reset, Pin 7: Set
    assert_int_equal(GPIO_Port.IDR, GPIO_PIN_0 | GPIO_PIN_7);  // Expected IDR
    assert_int_equal(GPIO_Port.ODR, GPIO_PIN_0 | GPIO_PIN_7);  // Expected ODR
    assert_int_equal(GPIO_Port.BSRR, GPIO_PIN_7); // Expected BSRR
}

// Test case: Attempt to write to an invalid GPIO pin
void test_hal_mock_write_pin_invalid_pin(void **state) {
    // Arrange: Initialize HAL and set initial values for GPIO registers
    hal_initialized = 1;
    GPIO_Port.IDR = GPIO_PIN_RESET;
    GPIO_Port.ODR = GPIO_PIN_RESET;
    GPIO_Port.BSRR = GPIO_PIN_0;
    uint16_t invalid_pin = 0x0000;

    // Act: Attempt to write to an invalid GPIO pin
    HAL_GPIO_WritePin(&GPIO_Port, invalid_pin, GPIO_PIN_SET);

    // Assert: Verify that writing to an invalid pin has no effect on IDR, ODR, and BSRR
    assert_int_equal(GPIO_Port.IDR, GPIO_PIN_RESET);
    assert_int_equal(GPIO_Port.ODR, GPIO_PIN_RESET);
    assert_int_equal(GPIO_Port.BSRR, GPIO_PIN_0);
}

// Test case: Verify behavior for null GPIO_TypeDef in HAL_GPIO_WritePin
void test_hal_mock_write_pin_null_input(void **state) {
    // Arrange: Initialize HAL and set initial values for GPIO registers
    hal_initialized = 1;
    GPIO_Port.IDR = GPIO_PIN_RESET;
    GPIO_Port.ODR = GPIO_PIN_RESET;
    GPIO_Port.BSRR = GPIO_PIN_0;

    // Act: Attempt to write to a pin with null GPIO_TypeDef
    HAL_GPIO_WritePin(NULL, GPIO_PIN_5, GPIO_PIN_SET);

    // Assert: Verify that writing to a pin with null GPIO_TypeDef has no effect on IDR, ODR, and BSRR
    assert_int_equal(GPIO_Port.IDR, 0);
    assert_int_equal(GPIO_Port.ODR, 0);
    assert_int_equal(GPIO_Port.BSRR, GPIO_PIN_0);
}

// Test case: Write to a GPIO pin with uninitialized HAL
void test_hal_mock_write_pin_no_hal_init(void **state) {
    // Arrange: Set HAL as uninitialized and set initial values for GPIO registers
    hal_initialized = 0;
    GPIO_Port.IDR = GPIO_PIN_RESET;
    GPIO_Port.ODR = GPIO_PIN_RESET;
    GPIO_Port.BSRR = GPIO_PIN_0;

    // Act: Write to a GPIO pin with uninitialized HAL
    HAL_GPIO_WritePin(&GPIO_Port, GPIO_PIN_5, GPIO_PIN_SET);

    // Assert: Verify that writing to a pin with uninitialized HAL has no effect on IDR, ODR, and BSRR
    assert_int_equal(GPIO_Port.IDR, GPIO_PIN_RESET);
    assert_int_equal(GPIO_Port.ODR, GPIO_PIN_RESET);
    assert_int_equal(GPIO_Port.BSRR, GPIO_PIN_0);
}
