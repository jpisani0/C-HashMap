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

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) // 64 bit architecture
    #define hash_func(key, len, seed, hash) MurmurHash3_x64_128(key, len, seed, hash)
#else // 32 bit architecture
    #define hash_func(key, len, seed, hash) MurmurHash3_x86_32(key, len, seed, hash)
#endif

//---------------------------------------------------------------------------------------------------------

// Key-value pair in each bucket for collisions
typedef struct node
{
    char* key;          // Key of this node
    void* value;        // Value of this node
    struct node* next;  // Pointer to next node in this linked list in the event of a collision
}node_t;

// Bucket for an index of the map
typedef struct bucket
{
    node_t* head;  // Pointer to the head of the linked list for this bucket
}bucket_t;

// The hashmap structure. Obfuscated from the user
struct hashmap
{
    size_t capacity;    // Maximum capacity of the map
    size_t size;        // Current size of the map
    bucket_t* buckets;  // Array of buckets for the map
    uint32_t seed;      // Seed for the hashes
};

//---------------------------------------------------------------------------------------------------------

/**
 * @brief Creates the hashmap_t object and returns the handle
 * 
 * @param capacity - max number of unique indexes for the map 
 * @return hashmap_t* - pointer to the hashmap_t object. NULL if error.
 */
hashmap_t* hashmap_create(size_t capacity)
{
    hashmap_t* map = (hashmap_t *)calloc(1, sizeof(hashmap_t));

    if(map != NULL)
    {
        map->capacity = capacity;
        map->size = 0;
        map->buckets = (bucket_t *)calloc(capacity, sizeof(bucket_t));
        map->seed = 0;
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
    if(map != NULL)
    {
        // Index through the linked list of all buckets
        for(size_t bucket_idx = 0; bucket_idx < map->capacity; bucket_idx++)
        {
            node_t* current = map->buckets[bucket_idx].head;

            while(current != NULL)
            {
                node_t* next = current->next;

                free(current->key);
                if(func != NULL) func(current->value);
                free(current);
                current = next;
            }
        }

        free(map->buckets);
        free(map);
    }
}

/**
 * @brief Add a new key-value pair to the map
 * 
 * @param map - pointer to the map
 * @param key - key for the pair
 * @param value - value for the pair
 * @return STATUS 
 */
STATUS hashmap_push(hashmap_t* map, const char* key, void* value)
{
    if(map == NULL || key == NULL || value == NULL) return ERROR;

    uint32_t hash[4] = {0};
    size_t bucket_idx = 0;
    node_t* new_node = NULL;
    new_node = (node_t *)calloc(1, sizeof(node_t));

    // REVIEW: better error codes
    if(new_node == NULL) return ERROR;

    hash_func((void *)key, strlen(key), map->seed, hash);
    bucket_idx = hash[0] % map->capacity;

    new_node->key = strdup(key);
    new_node->value = value;
    new_node->next = map->buckets[bucket_idx].head;
    map->buckets[bucket_idx].head = new_node;
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
    if(map == NULL || key == NULL) return NULL;

    uint32_t hash[4] = {0};
    hash_func((void *)key, strlen(key), map->seed, hash);
    size_t bucket_idx = hash[0] % map->capacity;
    node_t* current = map->buckets[bucket_idx].head;

    while((current != NULL) && (strcmp(current->key, key) != 0))
    {
        current = current->next;
    }

    if(current != NULL)
    {
        return current->value;
    }

    return NULL;
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
    if(map == NULL || key == NULL) return ERROR;

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

    if(current == NULL) return ERROR;

    // Make next of prev to next of current to remove from list
    prev->next = current->next;

    // Free current
    free(current->key);
    if(func != NULL) func(current->value);
    free(current);

    return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------
