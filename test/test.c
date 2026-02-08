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
#include <string.h>

#include "test.h"

extern const test_t __start_test_cases[];
extern const test_t __stop_test_cases[];


/**
 * @brief Entry point for the test driver
 * 
 * @return int 
 */
int main(void)
{
    const char* function_name = "hashmap_test():";
    STATUS error_status = SUCCESS;

    for(const test_t *t = __start_test_cases; t < __stop_test_cases; t++)
    {
        print_test_start(t->file_path, t->line_number, t->name);

        if(t->fn() == ERROR)
        {
            col_print("FAIL\n", COL_RED);
            error_status = ERROR;
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
