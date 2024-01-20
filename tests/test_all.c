#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "tests_hal_mock/tests_hal_mock.h"

int main(void) {
    const struct CMUnitTest hal_mock_general_tests[] = {
        cmocka_unit_test(test_hal_mock_hal_init_starts_at_zero),
        cmocka_unit_test(test_hal_mock_hal_init_changes_initialized),
        cmocka_unit_test(test_hal_mock_delay_zero),
        cmocka_unit_test(test_hal_mock_delay_short_duration),
        cmocka_unit_test(test_hal_mock_delay_long_duration),
        cmocka_unit_test(test_hal_mock_delay_consecutive),
        cmocka_unit_test(test_hal_mock_delay_no_hal_init),
    };

    cmocka_run_group_tests(hal_mock_general_tests, NULL, NULL);

    return 0;
}