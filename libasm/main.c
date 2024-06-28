#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define A1  "Holberton School"
#define A2  "Holberton"
#define B1  "Holberton"
#define B2  "Holberton School"

int main(void)
{
    // For the first case: asm_strncmp("Holberton School", "Holberton", strlen("Holberton"))
    printf("Case 1:\n");

    size_t N1 = strlen(A2);

    // Print values
    printf("a1: \"%s\"\n", A1);
    printf("a2: \"%s\"\n", A2);
    printf("N: %zu\n", N1);

    // Perform comparison
    assert(strncmp(A1, A2, N1) == asm_strncmp(A1, A2, N1));

    printf("All good!\n\n");

    // For the second case: asm_strncmp("Holberton", "Holberton School", strlen("Holberton"))
    printf("Case 2:\n");

    size_t N2 = strlen(B1);

    // Print values
    printf("b1: \"%s\"\n", B1);
    printf("b2: \"%s\"\n", B2);
    printf("N: %zu\n", N2);

    // Perform comparison
    assert(strncmp(B1, B2, N2) == asm_strncmp(B1, B2, N2));

    printf("All good!\n");

    return EXIT_SUCCESS;
}
