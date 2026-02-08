/**
 * @file test_hashmap_delete.c
 * @author J. Pisani (jgp9201@gmail.com)
 * @brief Testing functions for the hashmap_delete() library function
 * @version 0.1
 * @date 2025-12-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "test.h"


/**
 * @brief Test passing a null map or key to the hashmap_delete() function
 * @details Should return ERROR and errno should be HASHMAP_ERR_NULL_ARGS
 * 
 */
REGISTER_TEST(delete_with_null_values_test)
{
    STATUS error_status = SUCCESS;
    hashmap_t* map = hashmap_create(20);
    char* key = "key";

    // map is null
    if(hashmap_delete(NULL, key, NULL) != ERROR)
    {
        PRINT_ERR("hashmap_delete() did not return ERROR when map == NULL");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_NULL_ARG)
    {
        PRINT_ERR("errno is not HASHMAP_ERR_NULL_ARG");
        error_status = ERROR;
    }

    // key is null
    if(hashmap_delete(map, NULL, NULL) != ERROR)
    {
        PRINT_ERR("hashmap_delete() did not return ERROR when key == NULL");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_NULL_ARG)
    {
        PRINT_ERR("errno is not HASHMAP_ERR_NULL_ARG");
        error_status = ERROR;
    }

    hashmap_destroy(map, NULL);
    return error_status;
}

/**
 * @brief Test deleting an existing value with the hashmap_delete() function
 * @details Should return SUCCESS and errno should be HASHMAP_ERR_NONE
 * 
 */
REGISTER_TEST(delete_existing_value_test)
{
    STATUS error_status = SUCCESS;
    hashmap_t* map = hashmap_create(20);
    char* key = "key";
    int* value = (int *)malloc(sizeof(int));

    hashmap_push(map, key, value);

    if(hashmap_delete(map, key, free) != SUCCESS)
    {
        PRINT_ERR("hashmap_delete() did not return SUCCESS");
        error_status = ERROR;
    }

    if(hashmap_errno() != HASHMAP_ERR_NONE)
    {
        PRINT_ERR("errno is not HASHMAP_ERR_NONE");
        error_status = ERROR;
    }

    hashmap_destroy(map, NULL);
    return error_status;
}

/**
 * @brief Test deleting a non existent key with the hashmap_delete() function
 * @details Should return ERROR and errno should be HASHMAP_ERR_NOT_FOUND
 * 
 */
REGISTER_TEST(delete_non_existent_value_test)
{
    STATUS error_status = SUCCESS;
    hashmap_t* map = hashmap_create(20);
    char* key = "key";
    
    if(hashmap_delete(map, key, NULL) != ERROR)
    {
        PRINT_ERR("hashmap_delete() did not return ERROR");
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
