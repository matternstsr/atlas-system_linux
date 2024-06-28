/* Description: `asm_strcasecmp("Holberton", "")` */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1	"Holberton"
#define S2	""

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	assert(strcasecmp(S1, S2) == asm_strcasecmp(S1, S2));

	printf("All good!\n");
	return (EXIT_SUCCESS);
}