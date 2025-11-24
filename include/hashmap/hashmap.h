/**
 * @file hashmap.h
 * @author J. Pisani (jgp9201@gmail.com)
 * @brief API for using the hashmap
 * @version 0.1
 * @date 2025-11-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef _C_HASH_MAP_H
#define _C_HASH_MAP_H

#include <stddef.h>

typedef void (*free_value_func)(void *);
typedef struct hashmap hashmap_t;
typedef int STATUS;

#define SUCCESS 0
#define ERROR -1

typedef enum HASHMAP_ERROR_TYPE
{
    HASHMAP_ERR_NONE,           // No error
    HASHMAP_ERR_INVALID_CAPACITY,   // Invalid capacity for hashmap given (<1)
    HASHMAP_ERR_NULL_ARG,       // Null argument provided
    HASHMAP_ERR_ALLOC_FAILED,   // Memory allocation failed
    HASHMAP_ERR_KEY_EXISTS,     // Attempting to add key that already exists
    HASHMAP_ERR_NOT_FOUND,      // Key provided not found in the map
}hashmap_err_t;

hashmap_t* hashmap_create(size_t size);
void hashmap_destroy(hashmap_t* map, free_value_func func);
STATUS hashmap_push(hashmap_t* map, const char* key, void* value);
void* hashmap_get(const hashmap_t* map, const char* key);
STATUS hashmap_delete(hashmap_t* map, const char* key, free_value_func func);
void hashmap_set_seed(hashmap_t* map, size_t seed);
hashmap_err_t hashmap_errno(void);
const char* hashmap_strerror(hashmap_err_t errno);

#endif