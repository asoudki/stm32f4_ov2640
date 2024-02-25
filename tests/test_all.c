#include "tests_hal_mock/tests_hal_mock.h"
#include "tests_ov2640/test_ov2640.h"

int main(void) {    
    run_hal_mock_general_tests();
    run_hal_mock_gpio_tests();
    run_hal_mock_i2c_tests();
    run_hal_mock_spi_tests();

    run_ov2640_tests();

    return 0;
}