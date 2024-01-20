# tests/CMakeLists.txt

# Get test libraries
add_subdirectory(tests_hal_mock)

# Add the main executable
add_executable(test_all test_all.c)

# Link the executable with the test libraries
target_link_libraries(test_all PRIVATE test_hal_mock_general)

# Link libcmocka-static.a
target_link_libraries(test_all PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libcmocka-static.a)


