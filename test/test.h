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

#include "hashmap.h"
#include "color.h"

void print_test_start(const char* file_path, int line_number, const char* test_name);
const char* get_file_basename(const char* file_path);
void col_print(const char* str, const char* color);

typedef STATUS (*test_fn_t)(void);

typedef struct TEST
{
    const char* name;
    const char* file_path;
    int line_number;
    test_fn_t fn;
}test;

#ifdef TEST_SECTION
/**
 * @brief Register a test to the test_files section to be ran in test.c
 * 
 */
#define REGISTER_TEST_FILE(name) \
        extern const test __start_##name##_section[]; \
        extern const test __stop_##name##_section[]; \
        static STATUS name(void); \
        __attribute__((used, section("test_files"))) \
        static const test test_##name = { \
            #name, \
            __FILE__, \
            __LINE__, \
            name \
        }; \
        static STATUS name(void) \
        { \
            STATUS error_status = SUCCESS; \
            for(const test *t = __start_##name##_section; t < __stop_##name##_section; t++) { if(t->fn() == ERROR) { error_status = ERROR; } } \
            return error_status; \
        }

/**
 * @brief Register a test to the test file's section to be ran when its section in test_cases is called in test.c
 *        Each test file must define a section name with #define TEST_SECTION before including this file.
 * 
 *        ex) test_hashmap_create.c will have #define TEST_SECTION "tests_hashmap_create"
 * 
 */
#define REGISTER_TEST(name) \
        static STATUS name(void); \
        __attribute__((used, section(TEST_SECTION))) \
        static const test test_##name = { \
        #name, \
        __FILE__, \
        __LINE__, \
        name \
    }; \
    static STATUS name(void)
#endif

#endif
