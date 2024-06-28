#include <stdlib.h>
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
    printf("Case 1:asm_result1 = asm_strncmp(A1, A2, N1 \n");

    size_t N1 = strlen(A2);

    // Print values
    printf("a1: \"%s\"\n", A1);
    printf("a2: \"%s\"\n", A2);
    printf("N: %zu\n", N1);

    // Perform comparison
    int std_result1 = strncmp(A1, A2, N1);
    int asm_result1 = asm_strncmp(A1, A2, N1);

    if (std_result1 == asm_result1) {
        printf("Case 1: All good!\n\n");
    } else {
        printf("Case 1: You failed!\n");
    }

    // For the second case: asm_strncmp("Holberton", "Holberton School", strlen("Holberton"))
    printf("Case 2:asm_result2 = asm_strncmp(B1, B2, N2\n");

    size_t N2 = strlen(B1);

    // Print values
    printf("b1: \"%s\"\n", B1);
    printf("b2: \"%s\"\n", B2);
    printf("N: %zu\n", N2);

    // Perform comparison
    int std_result2 = strncmp(B1, B2, N2);
    int asm_result2 = asm_strncmp(B1, B2, N2);

    if (std_result2 == asm_result2) {
        printf("Case 2: All good!\n");
    } else {
        printf("Case 2: You failed!\n");
    }

    return EXIT_SUCCESS;
}
