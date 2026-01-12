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

#include "test.h"

extern const test __start_test_files[];
extern const test __stop_test_files[];

/**
 * @brief Entry point for the test driver
 * 
 * @return int 
 */
int main(void)
{
    const char* function_name = "hashmap_test():";
    STATUS error_status = SUCCESS;

    for(const test *t = __start_test_files; t < __stop_test_files; t++)
    {
        print_test_start(t->file_path, t->line_number, t->name);

        if(t->fn() == ERROR)
        {
            col_print("ERROR\n", COL_RED);
        }
        else
        {
            col_print("OK\n", COL_GREEN);
        }
    }

    if(error_status == ERROR)
    {
        printf("%s tests failed with errors\n", function_name);
    }
    else
    {
        printf("%s all tests passed successfully!\n", function_name);
    }

    return 0;
}