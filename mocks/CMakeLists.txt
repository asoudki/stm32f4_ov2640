# mocks/CMakeLists.txt

# Assuming this directory contains source files (.c)
add_library(hal_mock_general_lib hal_mock_general.c hal_mock_general.h)
add_library(hal_mock_gpio_lib hal_mock_gpio.c hal_mock_gpio.h)
add_library(hal_mock_i2c_lib hal_mock_i2c.c hal_mock_i2c.h)
add_library(hal_mock_spi_lib hal_mock_spi.c hal_mock_spi.h)

# Add include directories
target_include_directories(hal_mock_general_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(hal_mock_gpio_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(hal_mock_i2c_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(hal_mock_spi_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})