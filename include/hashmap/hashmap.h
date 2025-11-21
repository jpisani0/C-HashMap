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

hashmap_t* hashmap_create(size_t size);
void hashmap_destroy(hashmap_t* map, free_value_func func);
STATUS hashmap_push(hashmap_t* map, const char* key, const void* value);
void* hashmap_get(const hashmap_t* map, const char* key);
STATUS hashmap_delete(hashmap_t* map, const char* key, free_value_func func);

#endif