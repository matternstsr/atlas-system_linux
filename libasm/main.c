#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton School"
#define S2  "HOLBERTON SCHOOL"
#define S3  "Holberton SchooL"
#define S4  "holberton socool"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	printf("strcasecmp1: %d\n", strcasecmp(S1, S1));
	printf("strcasecmp2: %d\n", strcasecmp(S1, S2));
	printf("strcasecmp3: %d\n", strcasecmp(S1, S3));
	printf("strcasecmp4: %d\n", strcasecmp(S1, S4));
	printf("\n");
	printf("asm1: %d\n", asm_strcasecmp(S1, S1));
	printf("asm2: %d\n", asm_strcasecmp(S1, S2));
	printf("asm3: %d\n", asm_strcasecmp(S1, S3));
	printf("asm4: %d\n", asm_strcasecmp(S1, S4));

    assert(strcasecmp(S1, S1) == asm_strcasecmp(S1, S1));
    assert(strcasecmp(S1, S2) == asm_strcasecmp(S1, S2));
    assert(strcasecmp(S1, S3) == asm_strcasecmp(S1, S3));
    assert(strcasecmp(S1, S4) == asm_strcasecmp(S1, S4));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}









