# tests/tests_hal_mock/CMakeLists.txt

# Assuming this directory contains source files (.c)
add_library(test_hal_mock_general test_hal_mock_general.c test_hal_mock_general.h)

# Link libraries
target_link_libraries(test_hal_mock_general PRIVATE ov2640_lib hal_mock_general_lib hal_mock_gpio_lib hal_mock_i2c_lib hal_mock_spi_lib)