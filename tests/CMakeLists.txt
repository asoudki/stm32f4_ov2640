# Assuming this directory contains source files (.c)
add_executable(test_ov2640 test_ov2640.c)

# Link libraries
target_link_libraries(test_ov2640 PRIVATE ov2640_lib hal_mock_general_lib hal_mock_gpio_lib hal_mock_i2c_lib hal_mock_spi_lib)

# Add include directories
target_include_directories(test_ov2640 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})