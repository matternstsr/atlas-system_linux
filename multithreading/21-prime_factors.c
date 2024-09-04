#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

/* Helper function to convert string to unsigned long */
unsigned long string_to_ulong(char const *s)
{
    char *endptr;
    unsigned long number = strtoul(s, &endptr, 10);
    if (*endptr != '\0')
    {
        fprintf(stderr, "Invalid number: %s\n", s);
        exit(EXIT_FAILURE);
    }
    return number;
}

/* Helper function to factorize the number */
list_t *factorize_number(unsigned long number)
{
    list_t *factors = list_create(10);
    if (!factors)
        return NULL;

    /* Factor out the number by 2 */
    while (number % 2 == 0)
    {
        list_add(factors, (void *)(uintptr_t)2);
        number /= 2;
    }

    /* Factor out the number by odd factors */
    for (unsigned long i = 3; i * i <= number; i += 2)
    {
        while (number % i == 0)
        {
            list_add(factors, (void *)(uintptr_t)i);
            number /= i;
        }
    }

    /* If number is a prime larger than 2 */
    if (number > 2)
        list_add(factors, (void *)(uintptr_t)number);

    return factors;
}

/* Main function to create a list of prime factors */
list_t *prime_factors(char const *s)
{
    unsigned long number = string_to_ulong(s);
    return factorize_number(number);
}




