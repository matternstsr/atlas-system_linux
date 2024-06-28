#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton School"
#define S2  "Holberton"
#define N   strlen(S1)

int main(void)
{
    // Print value of N
    printf("Value of N: %zu\n", N);

    // Print "asm_strncmp(S1, S2, N)"
    printf("asm_strncmp(\"%s\", \"%s\", %zu)\n", S1, S2, N);

    // Perform the comparison
    int std_result = strncmp(S1, S2, N);
    int asm_result = asm_strncmp(S1, S2, N);

    // Print the results
    printf("strncmp result: %d\n", std_result);
    printf("asm_strncmp result: %d\n", asm_result);

    // Compare results
    assert(std_result == asm_result);

    printf("All good!\n");
    return EXIT_SUCCESS;
}
