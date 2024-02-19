# tests/CMakeLists.txt

# Get test libraries from tests_hal_mock directory
add_subdirectory(tests_hal_mock)

# Define a list of test library names
set(TEST_LIBRARIES
    test_hal_mock_general
    test_hal_mock_gpio
    test_hal_mock_i2c
    test_hal_mock_spi
    # Add more test libraries as needed
)

# Add the main executable
add_executable(test_all test_all.c)

# Loop through the list of test libraries and link them with the executable
foreach(TEST_LIBRARY ${TEST_LIBRARIES})
    target_link_libraries(test_all PRIVATE ${TEST_LIBRARY})
endforeach()

# Link libcmocka-static.a
target_link_libraries(test_all PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libcmocka-static.a)