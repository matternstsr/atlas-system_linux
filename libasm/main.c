#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton School"
#define S2  "Holberton"
#define N   strlen(S2)

int main(void)
{
    // Print value of N
    printf("Value of N: %zu\n", N);

    // Print "strncmp(S1, S2, N)"
    printf("strncmp(\"%s\", \"%s\", %zu)\n", S1, S2, N);

    // Perform the comparison with standard strncmp
    int std_result = strncmp(S1, S2, N);

    // Print the result of standard strncmp
    printf("strncmp result: %d\n", std_result);

    // Print "asm_strncmp(S1, S2, N)"
    printf("asm_strncmp(\"%s\", \"%s\", %zu)\n", S1, S2, N);

    // Perform the comparison with asm_strncmp
    int asm_result = asm_strncmp(S1, S2, N);

    // Print the result of asm_strncmp
    printf("asm_strncmp result: %d\n", asm_result);

    // Compare results
    assert(std_result == asm_result);

    printf("All good!\n");
    return EXIT_SUCCESS;
}
