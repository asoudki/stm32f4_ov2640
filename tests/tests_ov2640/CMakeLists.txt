# tests/tests_ov2640/CMakeLists.txt

# Define a list of test library names
set(TEST_LIBRARIES
    test_ov2640
)

# Define a list of library dependencies
set(LIB_DEPENDENCIES
    ov2640_lib
    hal_mock_general_lib
    hal_mock_gpio_lib
    hal_mock_i2c_lib
    hal_mock_spi_lib
    # Add more libraries as needed
)

# Loop through the list of test libraries
foreach(TEST_LIBRARY ${TEST_LIBRARIES})
    # Add the test library target
    add_library(${TEST_LIBRARY} ${TEST_LIBRARY}.c ${TEST_LIBRARY}.h)

    # Link the test library with other required libraries
    target_link_libraries(${TEST_LIBRARY} PRIVATE ${LIB_DEPENDENCIES})
endforeach()
