#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "tests_hal_mock/tests_hal_mock.h"

int main(void) {
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

    const struct CMUnitTest hal_mock_gpio_tests[] = {
        // HAL_GPIO_Init Tests
        cmocka_unit_test(test_hal_mock_gpio_init_returns_ok),
        cmocka_unit_test(test_hal_mock_gpio_init_sets_regs),
        cmocka_unit_test(test_hal_mock_gpio_init_no_hal_init),

        // HAL_GPIO_ReadPin Tests
        cmocka_unit_test(test_hal_mock_read_pin_reset_state),
        cmocka_unit_test(test_hal_mock_read_pin_set_state),
        cmocka_unit_test(test_hal_mock_read_pin_multiple),
        cmocka_unit_test(test_hal_mock_read_pin_invalid_pin),
        cmocka_unit_test(test_hal_mock_read_pin_no_hal_init),

        // HAL_GPIO_WritePin Tests
        cmocka_unit_test(test_hal_mock_write_pin_set_state),
        cmocka_unit_test(test_hal_mock_write_pin_reset_state),
        cmocka_unit_test(test_hal_mock_write_pin_multiple),
        cmocka_unit_test(test_hal_mock_write_pin_invalid_pin),
        cmocka_unit_test(test_hal_mock_write_pin_no_hal_init),
    };


    //cmocka_run_group_tests(hal_mock_general_tests, NULL, NULL);
    cmocka_run_group_tests(hal_mock_gpio_tests, NULL, NULL);

    return 0;
}