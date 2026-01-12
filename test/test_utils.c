/**
 * @file test_utils.c
 * @author J. Pisani (jgp9201@gmail.com)
 * @brief Utilities for testing
 * @version 0.1
 * @date 2026-01-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <stdio.h>
#include <string.h>

#include "test.h"

#define MAX_STRING 128


/**
 * @brief Prints the test start message
 * 
 * @param file_path
 * @param line_number
 * @param test_name
 */
inline void print_test_start(const char* file_path, int line_number, const char* test_name)
{
    char buffer[MAX_STRING] = {0};

    snprintf(buffer, MAX_STRING, "%s", get_file_basename(file_path));
    col_print(buffer, COL_BLUE);

    printf(":");

    snprintf(buffer, MAX_STRING, "%d", line_number);
    col_print(buffer, COL_YELLOW);

    printf(": Starting test: ");

    snprintf(buffer, MAX_STRING, "%s  ", test_name);
    col_print(buffer, COL_MAGENTA);
}

/**
 * @brief Print with color
 * 
 * @param str 
 * @param color
 */
inline void col_print(const char* str, const char* color)
{
    printf("%s%s", color, str);

    // Reset the color afterward
    printf(COL_RESET);
}

/**
 * @brief Get the basename from a given file path
 * 
 * @param file_path 
 * @return const char* 
 */
inline const char* get_file_basename(const char* file_path)
{
    const char* slash = strrchr(file_path, '/'); // Find the pointer to the last slash in the file path
    return slash ? slash + 1 : file_path; // Return the pointer to the first char in the file base name if found by strrchr(), full file path if not
}
