# Collect all source files
file(GLOB SRC_FILES *.c)

# TODO: add shared library alternative?
# Create the static library
add_library(hashmap STATIC ${SRC_FILES})

# Add the API header include directory
target_include_directories(hashmap PUBLIC ${CMAKE_SOURCE_DIR}/include/hashmap)

# Add the murmur3 hash library
add_subdirectory(murmur3)
