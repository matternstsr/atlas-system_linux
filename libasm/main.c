#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Declaration of assembly function
extern size_t asm_strspn(const char *s, const char *accept);

#define S1  "Holberton"
#define S2  "holberton"
#define S3  "HOLBERTON"

#define A1  "abcdefghijklmnopqrstuvwxyz"
#define A2  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    // Test cases with assert statements
    assert(strspn(S2, A1) == asm_strspn(S2, A1));
    assert(strspn(S2, A2) == asm_strspn(S2, A2));
    assert(strspn(S3, A1) == asm_strspn(S3, A1));
    assert(strspn(S3, A2) == asm_strspn(S3, A2));
    assert(strspn(S1, A1) == asm_strspn(S1, A1));
    assert(strspn(S1, A2) == asm_strspn(S1, A2));
    assert(strspn(S1, A1 A2) == asm_strspn(S1, A1 A2));

    // Additional test cases for coverage
    assert(asm_strspn("", A1) == 0); // Empty source string
    assert(asm_strspn("Holberton School", "") == 0); // Empty accept string
    assert(asm_strspn("", "") == 0); // Both strings empty
    assert(asm_strspn(S1, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") == strlen(S1)); // Full match including space

    printf("All tests passed!\n");

    return EXIT_SUCCESS;
}
