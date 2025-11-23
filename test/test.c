/**
 * @file test.c
 * @author J. Pisani (jgp9201@gmail.com)
 * @brief Test driver for the hashmap library
 * @version 0.1
 * @date 2025-11-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <stdio.h>

#include "hashmap.h"

#define PASS 0
#define FAILURE -1

const char* separator = "-----------------------------------------\n";

/**
 * @brief Entry point for the test driver
 * 
 * @return int 
 */
int main()
{
    const char* function_name = "hashmap_test():";

    hashmap_t* map = hashmap_create(20000);

    if(map == NULL) return FAILURE;

    return 0;
}