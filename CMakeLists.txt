# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(hashmap VERSION 1.0)

# Gather library source files
set(HASHMAP_SRC_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hashmap.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/murmur3/murmur3.c
)

# Create the library for hashmap
add_library(hashmap STATIC ${HASHMAP_SRC_FILES})

# Disable warnings for murmur3 only (third-party library)
set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/murmur3/murmur3.c
    PROPERTIES
    COMPILE_OPTIONS "-w"
)

# Set include directories
target_include_directories(hashmap 
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include/hashmap 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/murmur3
)

# Enable strict warnings
if (MSVC)
    target_compile_options(hashmap PRIVATE /W4)
else()
    target_compile_options(hashmap PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set the output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    # Gather example source files
    file(GLOB EXAMPLE_SRC_FILES ${CMAKE_SOURCE_DIR}/example/*.c)

    # Gather test source files
    file(GLOB TEST_SRC_FILES ${CMAKE_SOURCE_DIR}/test/*.c)

    # Create example binary
    add_executable(hashmap_example ${EXAMPLE_SRC_FILES})
    target_link_libraries(hashmap_example PRIVATE hashmap)
    target_include_directories(hashmap_example PRIVATE include/hashmap)

    # Create test binary
    add_executable(hashmap_test ${TEST_SRC_FILES})
    target_link_libraries(hashmap_test PRIVATE hashmap)
    target_include_directories(hashmap_test PRIVATE include/hashmap)
endif()
