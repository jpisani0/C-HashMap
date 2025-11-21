# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(hashmap VERSION 1.0)

# Set the output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add debug prints compile definition for debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(HASHMAP_DEBUG)
endif()

# REVIEW: need for sub directories? manageable enough with 1 cmakelists i think
# Add the source and test directories
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(example)