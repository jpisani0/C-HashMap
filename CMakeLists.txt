# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(hashmap VERSION 1.0)

# Enable strict warnings
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Set the output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add debug prints compile definition for debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(HASHMAP_DEBUG)
endif()

# Create the library for hashmap
add_library(hashmap STATIC 
    src/hashmap.c 
    src/murmur3/murmur3.c
)

target_include_directories(hashmap 
    PUBLIC include/hashmap 
    PRIVATE src/murmur3
)   

# Create test binary
add_executable(hashmap_test test/test.c)
target_link_libraries(hashmap_test PRIVATE hashmap)
target_include_directories(hashmap_test PRIVATE include/hashmap)

# Create example binary
add_executable(hashmap_example example/example.c)
target_link_libraries(hashmap_example PRIVATE hashmap)
target_include_directories(hashmap_example PRIVATE include/hashmap)

# if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
