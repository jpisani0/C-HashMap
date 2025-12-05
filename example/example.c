/**
 * @file example.c
 * @author J. Pisani (jgp9201@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-11-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashmap.h"

typedef struct
{
    int acc_number;
    char* name;
    char* email;
}Account;

STATUS setupAndAddAccounts(hashmap_t* map);
void freeAccount(void* acc);


/**
 * @brief Driver for the example program
 * 
 * @return int 
 */
int main(void)
{
    const char* function_name = "hashmap_example():";
    hashmap_t* map = hashmap_create(3);

    if(map == NULL || setupAndAddAccounts(map) == ERROR) return -1;

    Account* acc1 = (Account *)hashmap_get(map, "jsmith@email.com");

    if(acc1 != NULL)
    {
        printf("%s acc1 account number: %d\n", function_name, acc1->acc_number);
        printf("%s acc1 account name: %s\n", function_name, acc1->name);
        printf("%s acc1 email: %s\n", function_name, acc1->email);
    }

    Account* acc2 = (Account *)hashmap_get(map, "jdoe@email.com");

    if(acc2 != NULL)
    {
        printf("%s acc2 account number: %d\n", function_name, acc2->acc_number);
        printf("%s acc2 account name: %s\n", function_name, acc2->name);
        printf("%s acc2 email: %s\n", function_name, acc2->email);
    }

    Account* acc3 = (Account *)hashmap_get(map, "sstevenson@email.com");

    if(acc3 != NULL)
    {
        printf("%s acc3 account number: %d\n", function_name, acc3->acc_number);
        printf("%s acc3 account name: %s\n", function_name, acc3->name);
        printf("%s acc3 email: %s\n", function_name, acc3->email);
    }

    // Here we pass the void freeAccount(void*) function by reference to the hashmap_destroy() function for freeing our values. 
    // If your value is simple, you can just pass free(). If your value is not dynamically allocated, you can pass NULL to not free anything.
    hashmap_destroy(map, freeAccount);
    map = NULL;

    return 0;
}

/**
 * @brief Setup account structures to be placed in hashmap
 * 
 * @param map - pointer to the map
 * @return STATUS - SUCCESS or ERROR
 */
STATUS setupAndAddAccounts(hashmap_t* map)
{
    Account* acc1 = (Account *)malloc(sizeof(Account));
    Account* acc2 = (Account *)malloc(sizeof(Account));
    Account* acc3 = (Account *)malloc(sizeof(Account));

    if(acc1 == NULL || acc2 == NULL || acc3 == NULL) return ERROR;

    acc1->acc_number = 1;
    acc1->name = strdup("John Smith");
    acc1->email = strdup("jsmith@email.com");

    acc2->acc_number = 2;
    acc2->name = strdup("Jane Doe");
    acc2->email = strdup("jdoe@email.com");

    acc3->acc_number = 3;
    acc3->name = strdup("Steven Stevenson");
    acc3->email = strdup("sstevenson@email.com");

    return (hashmap_push(map, acc1->email, (void *)acc1) || hashmap_push(map, acc2->email, (void *)acc2) || hashmap_push(map, acc3->email, (void *)acc3));
}

/**
 * @brief Frees the account struct
 * 
 * @param ptr - pointer to the Account struct to be freed
 */
void freeAccount(void* ptr)
{
    Account* acc = (Account *)ptr;
    free(acc->name);
    free(acc->email);
	free(acc);
}
