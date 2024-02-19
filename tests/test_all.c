#include "tests_hal_mock/tests_hal_mock.h"

int main(void) {    
    run_hal_mock_general_tests();
    run_hal_mock_gpio_tests();
    run_hal_mock_i2c_tests();
    run_hal_mock_spi_tests();

    return 0;
}