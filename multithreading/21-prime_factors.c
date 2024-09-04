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

	if (num == 0)
	{
		fprintf(stderr, "Invalid number: %s\n", s);
		return NULL;
	}
	unsigned long *temp;
	unsigned long primenum = 2;
	list_t *prime_list = malloc(sizeof(list_t));
	list_init(prime_list);

	while (primenum * primenum <= num)
	{
		while (num % primenum == 0)
		{
			temp = malloc(sizeof(unsigned long));
			if (!temp)
			{
				perror("Failed to allocate memory for prime factor");
				list_destroy(prime_list, free);
				exit(EXIT_FAILURE);
			}
			*temp = primenum;
			list_add(prime_list, temp);
			num /= primenum;
		}
		primenum += 1 + (primenum != 2);
	}
	if (num >= 2)
	{
		temp = malloc(sizeof(unsigned long));
		if (!temp)
		{
			perror("Failed to allocate memory for prime factor");
			list_destroy(prime_list, free);
			exit(EXIT_FAILURE);
		}
		*temp = num;
		list_add(prime_list, temp);
	}
	return prime_list;
}
