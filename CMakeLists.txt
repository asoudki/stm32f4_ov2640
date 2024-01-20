# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(ov2640_project)

# Define USE_MOCK_HAL when using test build
add_definitions(-DUSE_MOCK_HAL)

# Include the ov2640 folder
add_subdirectory(ov2640)
# Include the mocks folder
add_subdirectory(mocks)
# Include the ov2640_tests folder
add_subdirectory(ov2640_tests)