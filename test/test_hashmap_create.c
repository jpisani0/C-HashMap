/**
 * @file test_hashmap_create.c
 * @author J. Pisani (jgp9201@gmail.com)
 * @brief Testing functions for the hashmap_create() function
 * @version 0.1
 * @date 2025-12-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "test.h"


/**
 * @brief Test passing a capacity of 0 to hashmap_create().
 * @details A capacity of 0 is invalid and should return NULL and errno should be HASHMAP_ERR_INVALID_CAPACITY
 * 
 * @return STATUS - SUCCESS or ERROR
 */
REGISTER_TEST(zero_capacity_test)
{
    STATUS error_status = SUCCESS;
    hashmap_t* map = hashmap_create(0);

    if(map != NULL) 
    { 
        PRINT_ERR("map is not null");
        error_status = ERROR; 
    }

    if(hashmap_errno() != HASHMAP_ERR_INVALID_CAPACITY)
    {
        PRINT_ERR("errno is not HASHMAP_ERR_INVALID_CAPCITY");
        error_status = ERROR;
    }

    hashmap_destroy(map, NULL);

    return error_status;
}

/**
 * @brief Test passing a capcity greater than 2^32 to hashmap_create()
 * @details A capacity larger than 2^32 is invalid as the hashing algorithm only supports an output of 
 * 32 bits on 32 bit architecture and we only take the first 32 bits on 64 bit architecture.
 * Should return NULL and errno should be HASHMAP_ERR_INVALID_CAPACITY
 * 
 * @return STATUS - SUCCESS or ERROR
 */
REGISTER_TEST(large_capacity_test)
{
    const char* function_name = "test_large_capacity():";
    hashmap_t* map = hashmap_create((size_t)4294967297UL);
    STATUS error_status = SUCCESS;

    if(map != NULL)
    {
        PRINT_ERR("map is not null");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_INVALID_CAPACITY)
    {
        PRINT_ERR("errno is not HASHMAP_ERR_INVALID_CAPACITY");
        error_status = ERROR;
    }

    hashmap_destroy(map, NULL);

    return error_status;
}
