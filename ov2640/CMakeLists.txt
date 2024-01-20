# Set your source files
set(SOURCES
    ov2640.c
    ov2640_regs.c
)

# Create a static library from the source files
add_library(ov2640_lib STATIC ${SOURCES})

# Include the current directory as an interface include directory
target_include_directories(ov2640_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Check if USE_MOCK_HAL is defined
if(USE_MOCK_HAL)
    # Link the mock library when using the mock HAL
    target_link_libraries(ov2640_lib PRIVATE mock_lib)
endif()