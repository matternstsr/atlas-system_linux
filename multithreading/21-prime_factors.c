#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

/**
 * prime_factors - Factors a given number into a list containing prime factors
 *
 * @s: string of the number to factor
 * Return: list of prime factors
 **/
list_t *prime_factors(char const *s)
{
    unsigned long num = strtoul(s, NULL, 10);
    if (num == 0) {
        fprintf(stderr, "Invalid number: %s\n", s);
        return NULL;
    }

    unsigned long *temp;
    unsigned long prime = 2;
    list_t *prime_list = list_init();

    while (prime * prime <= num)
    {
        while (num % prime == 0)
        {
            temp = malloc(sizeof(unsigned long));
            if (!temp) {
                perror("Failed to allocate memory for prime factor");
                list_destroy(prime_list, free);
                exit(EXIT_FAILURE);
            }
            *temp = prime;
            list_add(prime_list, temp);
            num /= prime;
        }

        prime += 1 + (prime != 2);
    }

    if (num >= 2)
    {
        temp = malloc(sizeof(unsigned long));
        if (!temp) {
            perror("Failed to allocate memory for prime factor");
            list_destroy(prime_list, free);
            exit(EXIT_FAILURE);
        }
        *temp = num;
        list_add(prime_list, temp);
    }

    return prime_list;
}
