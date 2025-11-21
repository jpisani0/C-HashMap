# Collect all source files
file(GLOB TEST_FILES *.c)

# TODO: add shared library alternative?
# Create test executable
add_executable(hashmap_test ${TEST_FILES})

# Link library
target_link_libraries(hashmap_test hashmap)

# Add API header include directory
target_include_directories(hashmap_test PRIVATE ${CMAKE_SOURCE_DIR}/include/hashmap)
