/**
 * @file hashmap.c
 * @author J. Pisani (jgp9201@gmail.com)
 * @brief Contains functions for accessing and manipulating the hashmap
 * @version 0.1
 * @date 2025-11-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
#include "murmur3.h"

#define MAX_HASHMAP_CAPACITY 4294967296 // 2^32. In 32 bit arch, hashing algo only outputs 32 bit hashes. In 64 bit arch, we only use the first 32 bits of the 128 bit output.

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) // 64 bit architecture
    #define hash_func(key, len, seed, hash) MurmurHash3_x64_128(key, len, seed, hash)
#else // 32 bit architecture
    #define hash_func(key, len, seed, hash) MurmurHash3_x86_32(key, len, seed, hash)
#endif

//---------------------------------------------------------------------------------------------------------

typedef struct node node_t;
typedef struct bucket bucket_t;

// Key-value pair in each bucket for collisions
struct node
{
    char* key;          // Key of this node
    void* value;        // Value of this node
    node_t* next;  // Pointer to next node in this linked list in the event of a collision
};

// Linked list for an index of the map
struct bucket
{
    node_t* head;  // Pointer to the head of the linked list for this bucket
};

// The hashmap structure. Obfuscated from the user
struct hashmap
{
    size_t capacity;    // Maximum capacity of the map
    size_t size;        // Current size of the map
    bucket_t* buckets;  // Array of buckets for the map
    uint32_t seed;      // Seed for the hashing algorithm
};

static hashmap_err_t errno = HASHMAP_ERR_NONE;  // Last error from the hashmap library initialized to HASHMAP_ERR_NONE

//---------------------------------------------------------------------------------------------------------

/**
 * @brief Creates the hashmap_t object and returns the handle
 * 
 * @param capacity - max number of unique indexes for the map 
 * @return hashmap_t* - pointer to the hashmap_t object. NULL if error.
 */
hashmap_t* hashmap_create(size_t capacity)
{
    errno = HASHMAP_ERR_NONE;
    hashmap_t* map = NULL;

    // Check for valid capacity
    if(capacity <= MAX_HASHMAP_CAPACITY)
    {
        map = (hashmap_t *)malloc(sizeof(hashmap_t));

        // Check memory allocation succeeded
        if(map != NULL)
        {
            map->capacity = capacity;
            map->size = 0;
            map->buckets = (bucket_t *)calloc(capacity, sizeof(bucket_t));
            map->seed = 0;

            // Check that the buckets array was allocated successfully
            if(map->buckets == NULL)
            {
                free(map);
                map = NULL;
                errno = HASHMAP_ERR_ALLOC_FAILED;
            }
        }
        else
        {
            errno = HASHMAP_ERR_ALLOC_FAILED;
        }
    }
    else
    {
        errno = HASHMAP_ERR_INVALID_CAPACITY;
    }

    return map;
}

/**
 * @brief Safely deallocates map. User can optionally provide a function for freeing values.
 * 
 * @param map - pointer to the map
 * @param func - optional pointer to function for freeing values. Can pass NULL to not have library handle freeing values.
 */
void hashmap_destroy(hashmap_t* map, free_value_func func)
{
    errno = HASHMAP_ERR_NONE;

    if(map != NULL)
    {
        if(map->buckets != NULL)
        {
            // Index through the linked list of all buckets
            for(size_t bucket_idx = 0; bucket_idx < map->capacity; bucket_idx++)
            {
                node_t* current = map->buckets[bucket_idx].head;

                while(current != NULL)
                {
                    node_t* next = current->next;

                    free(current->key);

                    if(func != NULL)
                    {
                        func(current->value);
                    }
                    else
                    {
                        // Default to standard free if user doesn't pass a free func
                        free(current->value);
                    }

                    free(current);
                    current = next;
                }
            }

            free(map->buckets);
        }

        free(map);
    }
}

// TODO: catch duplicates
/**
 * @brief Add a new key-value pair to the map
 * 
 * @param map - pointer to the map
 * @param key - key for the pair
 * @param value - value for the pair
 * @return STATUS 
 */
STATUS hashmap_push(hashmap_t* map, const char* key, const void* value)
{
    if(map == NULL || key == NULL || value == NULL)
    {
        errno = HASHMAP_ERR_NULL_ARG;
        return ERROR;
    }

    errno = HASHMAP_ERR_NONE;
    uint32_t hash[4] = {0};
    size_t bucket_idx = 0;
    node_t* node = NULL;
    node = (node_t *)malloc(sizeof(node_t));

    if(node == NULL)
    {
        errno = HASHMAP_ERR_ALLOC_FAILED;
        return ERROR;
    }

    hash_func((void *)key, strlen(key), map->seed, hash);
    bucket_idx = hash[0] % map->capacity;

    // Make a copy of the key
    node->key = strdup(key);

    if(node->key == NULL)
    {
        free(node);
        errno = HASHMAP_ERR_ALLOC_FAILED;
        return ERROR;
    }

    // Make a copy of the value
    node->value = malloc(sizeof(value));

    if(node->value == NULL)
    {
        free(node->key);
        free(node);
        errno = HASHMAP_ERR_ALLOC_FAILED;
        return ERROR;
    }

    memcpy(node->value, value, sizeof(value));

    // Insert into bucket at head of linked list
    node->next = map->buckets[bucket_idx].head;
    map->buckets[bucket_idx].head = node;
    map->size++;

    return SUCCESS;
}

/**
 * @brief Returns the value for the given key
 * 
 * @param map - pointer to the map
 * @param key - key to search for 
 * @return void* - NULL if not found, pointer to value otherwise
 */
void* hashmap_get(const hashmap_t* map, const char* key)
{
    if(map == NULL || key == NULL)
    {
        errno = HASHMAP_ERR_NULL_ARG;
        return NULL;
    }

    errno = HASHMAP_ERR_NONE;
    uint32_t hash[4] = {0};
    hash_func((void *)key, strlen(key), map->seed, hash);
    size_t bucket_idx = hash[0] % map->capacity;
    node_t* current = map->buckets[bucket_idx].head;

    // Loop until we reach the end of the list (key not found) or we find a node with the key
    while((current != NULL) && (strcmp(current->key, key) != 0))
    {
        current = current->next;
    }

    return current->value;
}

/**
 * @brief Deletes a key-value pair from the map
 * 
 * @param map - pointer to the map
 * @param key - key to be deleted
 * @param func - optional function for freeing the value. Can be left NULL if user plans to handle deallocation.
 * @return STATUS 
 */
STATUS hashmap_delete(hashmap_t* map, const char* key, free_value_func func)
{
    if(map == NULL || key == NULL)
    {
        errno = HASHMAP_ERR_NULL_ARG;
        return ERROR;
    }

    errno = HASHMAP_ERR_NONE;
    uint32_t hash[4] = {0};
    hash_func((void *)key, strlen(key), map->seed, hash);
    size_t bucket_idx = hash[0] % map->capacity;
    node_t* current = map->buckets[bucket_idx].head;
    node_t* prev = NULL;

    // Find the node to be deleted
    while((current != NULL) && (strcmp(current->key, key) != 0))
    {
        prev = current;
        current = current->next;
    }

    if(current == NULL)
    {
        errno = HASHMAP_ERR_NOT_FOUND;
        return ERROR;
    }

    // Make next of prev to next of current to remove from list
    prev->next = current->next;

    // Free current
    free(current->key);

    if(func != NULL)
    {
        func(current->value);
    }
    else
    {
        // Default to free() if no free func provided
        free(current->value);
    }

    free(current);

    return SUCCESS;
}

/**
 * @brief Set the seed for this map
 * 
 * @param map - pointer to the map
 * @param seed - the seed for the map
 */
void hashmap_set_seed(hashmap_t* map, size_t seed)
{
    map->seed = seed;
}

/**
 * @brief Returns the current errno
 * 
 * @return hashmap_err_t - the errno enum
 */
hashmap_err_t hashmap_errno(void)
{
    return errno;
}

/**
 * @brief Returns the current errno in string format
 * 
 * hashmap_err_t errno - the error number
 * @return const char* - error number in string format
 */
const char* hashmap_strerror(hashmap_err_t errno)
{
    switch(errno)
    {
        case HASHMAP_ERR_NONE:          return (char *)"NO ERROR";
        case HASHMAP_ERR_NULL_ARG:      return (char *)"NULL ARGUMENT";
        case HASHMAP_ERR_ALLOC_FAILED:  return (char *)"MEMORY ALLOCATION FAILURE";
        case HASHMAP_ERR_KEY_EXISTS:    return (char *)"KEY ALREADY EXISTS";
        case HASHMAP_ERR_NOT_FOUND:     return (char *)"KEY NOT FOUND";
        default:                        return (char *)"UNKNOWN ERROR";
    }
}

//---------------------------------------------------------------------------------------------------------
