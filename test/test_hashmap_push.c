/**
 * @file test_hashmap_push.c
 * @author J. Pisani (jgp9201@gmail.com)
 * @brief Testing functions for the hashmap_push() library function
 * @version 0.1
 * @date 2025-12-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "test.h"


/**
 * @brief Test passing a null value in any input parameter to the hashmap_push() function
 * @details Should return ERROR and errno should be HASHMAP_ERR_NULL_ARG
 */
REGISTER_TEST(push_null_values_test)
{
    STATUS error_status = SUCCESS;
    hashmap_t* map = hashmap_create(20);
    char* key = "key";
    int* value = (int *)malloc(sizeof(int));

    // map is NULL
    if(hashmap_push(NULL, key, value) != ERROR)
    {
        PRINT_ERR("hashmap_push() did not return ERROR with map == NULL");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_NULL_ARG)
    {
        PRINT_ERR("errno is not HASHMAP_ERR_NULL_ARG");
        error_status = ERROR;
    }

    // key is NULL
    if(hashmap_push(map, NULL, value) != ERROR)
    {
        PRINT_ERR("hashmap_push() did not return ERROR with key == NULL");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_NULL_ARG)
    {
        PRINT_ERR("errno is not HASHMAP_ERR_NULL_ARG");
        error_status = ERROR;
    }

    // value is NULL
    if(hashmap_push(map, key, NULL) != ERROR)
    {
        PRINT_ERR("hashmap_push() did not return ERROR with value == NULL");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_NULL_ARG)
    {
        PRINT_ERR("errno is not HASHMAP_ERR_NULL_ARG");
        error_status = ERROR;
    }

    hashmap_destroy(map, free);
    free((void *)value);

    return error_status;
}

/**
 * @brief Test passing a duplicate key to the hashmap_push() function
 * @details Should return ERROR and errno should be HASHMAP_ERR_DUPLICATE
 * 
 */
REGISTER_TEST(duplicate_key_test)
{
    STATUS error_status = SUCCESS;
    hashmap_t* map = hashmap_create(20);
    char* key = "key";
    int* value = (int *)malloc(sizeof(int));

    hashmap_push(map, key, value);

    // Push same key again
    if(hashmap_push(map, key, value) != ERROR)
    {
        PRINT_ERR("hashmap_push() did not return ERROR with duplicate key");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_DUPLICATE)
    {
        PRINT_ERR("errno is not HASHAMP_ERR_DUPLICATE");
        error_status = ERROR;
    }

    hashmap_destroy(map, free);

    return error_status;
}


