/**
 * @file test_hashmap_get.c
 * @author J. Pisani (jgp9201@gmail.com)
 * @brief Testing functions for the hashmap_get() library function
 * @version 0.1
 * @date 2025-12-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "test.h"


/**
 * @brief Test passing null values to the hashmap_get() function
 * @details Should return NULL and errno should be HASHMAP_ERR_NULL_ARGS
 * 
 */
REGISTER_TEST(get_with_null_values_test)
{
    STATUS error_status = SUCCESS;
    hashmap_t* map = hashmap_create(20);
    char* key = "key";
    int* value = malloc(sizeof(int));

    hashmap_push(map, key, (void *)value);

    // map is NULL
    if(hashmap_get(NULL, key) != NULL)
    {
        PRINT_ERR("hashmap_get() did not return NULL when map == NULL");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_NULL_ARG)
    {
        PRINT_ERR("errno is not HASHMAP_ERR_NULL_ARG");
        error_status = ERROR;
    }

    // key is NULL
    if(hashmap_get(map, NULL) != NULL)
    {
        PRINT_ERR("hashmap_get() did not return NULL when key == NULL");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_NULL_ARG)
    {
        PRINT_ERR("errno is not HASHMAP_ERR_NULL_ARG");
        error_status = ERROR;
    }

    hashmap_destroy(map, free);
    return error_status;
}

/**
 * @brief Test getting an existing key with the hashmap_get() function
 * @details Should return the pointer to the value and errno should be HASHMAP_ERR_NONE
 * 
 */
REGISTER_TEST(get_existing_key_test)
{
    STATUS error_status = SUCCESS;
    hashmap_t* map = hashmap_create(20);
    char* key = "key";
    int* value = (int *)malloc(sizeof(int));

    hashmap_push(map, key, value);

    if(hashmap_get(map, key) != (void *)value)
    {
        PRINT_ERR("hashmap_get() did not return a pointer to value");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_NONE)
    {
        PRINT_ERR("errno is not HASHAMP_ERR_NONE");
        error_status = ERROR;
    }

    hashmap_destroy(map, free);
    return error_status;
}

/**
 * @brief Test getting a non existent key with the hashmap_get() function
 * @details Should return NULL and errno should be HASHMAP_ERR_NOT_FOUND
 * 
 */
REGISTER_TEST(get_non_existent_key_test)
{
    STATUS error_status = SUCCESS;
    hashmap_t* map = hashmap_create(20);
    char* key = "key";

    if(hashmap_get(map, key) != NULL)
    {
        PRINT_ERR("hashmap_get() did not return NULL");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_NOT_FOUND)
    {
        PRINT_ERR("errno is not HASHMAP_ERR_NOT_FOUND");
        error_status = ERROR;
    }

    hashmap_destroy(map, NULL);
    return error_status;
}
