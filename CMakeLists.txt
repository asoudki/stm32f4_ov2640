# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(ov2640_project)

# Define USE_MOCK_HAL when using test build
add_definitions(-DUSE_MOCK_HAL)

# Include subdirectories
add_subdirectory(ov2640)
add_subdirectory(mocks)
add_subdirectory(tests)
