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

# Gather library source files
file(GLOB HASHMAP_SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.c ${CMAKE_SOURCE_DIR}/src/**/*.c)

# Gather example source files
file(GLOB EXAMPLE_SRC_FILES ${CMAKE_SOURCE_DIR}/example/*.c)

# Gather test source files
file(GLOB TEST_SRC_FILES ${CMAKE_SOURCE_DIR}/test/*.c)

# Add debug prints compile definition for debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(HASHMAP_DEBUG)
endif()

# Create the library for hashmap
add_library(hashmap STATIC ${HASHMAP_SRC_FILES})
target_include_directories(hashmap PUBLIC include/hashmap PRIVATE src/murmur3)

# Create example binary
add_executable(hashmap_example ${EXAMPLE_SRC_FILES})
target_link_libraries(hashmap_example PRIVATE hashmap)
target_include_directories(hashmap_example PRIVATE include/hashmap)

# Create test binary
add_executable(hashmap_test ${TEST_SRC_FILES})
target_link_libraries(hashmap_test PRIVATE hashmap)
target_include_directories(hashmap_test PRIVATE include/hashmap)

# if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
