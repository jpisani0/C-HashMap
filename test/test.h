/**
 * @file test.h
 * @author J. Pisani (jgp9201@gmail.com)
 * @brief Contains testing functions for the library
 * @version 0.1
 * @date 2025-12-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef _TEST_HASHMAP_H
#define _TEST_HASHMAP_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "hashmap.h"
#include "color.h"

#define MAX_STRING 128

void print_test_start(const char* file_path, int line_number, const char* test_name);
const char* get_file_basename(const char* file_path);
void col_print(const char* str, const char* color);

typedef STATUS (*test_fn_t)(void);

/**
 * @brief Struct that holds information on the test
 * 
 */
typedef struct TEST
{
    const char* name;
    const char* file_path;
    int line_number;
    test_fn_t fn;
}test_t;

/**
 * @brief Register a test to the test_cases section to be called in test.c
 * 
 */
#define REGISTER_TEST(name) \
        static STATUS name(void); \
        __attribute__((used, section("test_cases"))) \
        static const test_t test_##name = { \
        #name, \
        __FILE__, \
        __LINE__, \
        name \
    }; \
    static STATUS name(void)

#define PRINT_ERR(err_msg) printf("\t[%s:%s()] %s\n", get_file_basename(__FILE__), __func__, err_msg)

#endif
