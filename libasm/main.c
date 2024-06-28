/* Description: `asm_strncmp("", "Holberton", strlen("Holberton"))` */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1	""
#define S2	"Holberton"
#define N	strlen(S2)

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	assert(strncmp(S1, S2, N) == asm_strncmp(S1, S2, N));

	printf("All good!\n");
	return (EXIT_SUCCESS);
}