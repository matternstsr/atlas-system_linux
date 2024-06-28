/* Description: `asm_putc('H');` */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define C	'H'

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	assert(asm_putc(C) == 1);

	return (EXIT_SUCCESS);
}