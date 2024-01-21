# mocks/CMakeLists.txt

# Define a list of mock library names without _lib extension
set(MOCK_LIBRARIES
    hal_mock_general
    hal_mock_gpio
    hal_mock_i2c
    hal_mock_spi
    # Add more mock libraries as needed
)

# Loop through the list of mock libraries
foreach(MOCK_LIBRARY ${MOCK_LIBRARIES})
    # Create a variable for the library name with _lib extension
    set(LIBRARY_NAME ${MOCK_LIBRARY}_lib)

    # Add the mock library target
    add_library(${LIBRARY_NAME} ${MOCK_LIBRARY}.c ${MOCK_LIBRARY}.h)

    # Add include directories for the mock library
    target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()
