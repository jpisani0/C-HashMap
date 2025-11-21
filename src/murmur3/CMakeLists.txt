# Create the static library 
add_library(murmur3 STATIC murmur3.c)

# Add the API header directory (here)
target_include_directories(hashmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
