#include <stdlib.h>
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
    // Test cases with print statements
    printf("Test Case: asm_strspn(\"%s\", \"%s\")\n", S2, A1);
    size_t result_asm = asm_strspn(S2, A1);
    size_t result_strspn = strspn(S2, A1);
    printf("asm_strspn result: %zu\n", result_asm);
    printf("strspn result: %zu\n", result_strspn);
    printf("Result: %s\n\n", result_asm == result_strspn ? "Passed" : "Failed");

    printf("Test Case: asm_strspn(\"%s\", \"%s\")\n", S2, A2);
    result_asm = asm_strspn(S2, A2);
    result_strspn = strspn(S2, A2);
    printf("asm_strspn result: %zu\n", result_asm);
    printf("strspn result: %zu\n", result_strspn);
    printf("Result: %s\n\n", result_asm == result_strspn ? "Passed" : "Failed");

    printf("Test Case: asm_strspn(\"%s\", \"%s\")\n", S3, A1);
    result_asm = asm_strspn(S3, A1);
    result_strspn = strspn(S3, A1);
    printf("asm_strspn result: %zu\n", result_asm);
    printf("strspn result: %zu\n", result_strspn);
    printf("Result: %s\n\n", result_asm == result_strspn ? "Passed" : "Failed");

    printf("Test Case: asm_strspn(\"%s\", \"%s\")\n", S3, A2);
    result_asm = asm_strspn(S3, A2);
    result_strspn = strspn(S3, A2);
    printf("asm_strspn result: %zu\n", result_asm);
    printf("strspn result: %zu\n", result_strspn);
    printf("Result: %s\n\n", result_asm == result_strspn ? "Passed" : "Failed");

    printf("Test Case: asm_strspn(\"%s\", \"%s\")\n", S1, A1);
    result_asm = asm_strspn(S1, A1);
    result_strspn = strspn(S1, A1);
    printf("asm_strspn result: %zu\n", result_asm);
    printf("strspn result: %zu\n", result_strspn);
    printf("Result: %s\n\n", result_asm == result_strspn ? "Passed" : "Failed");

    printf("Test Case: asm_strspn(\"%s\", \"%s\")\n", S1, A2);
    result_asm = asm_strspn(S1, A2);
    result_strspn = strspn(S1, A2);
    printf("asm_strspn result: %zu\n", result_asm);
    printf("strspn result: %zu\n", result_strspn);
    printf("Result: %s\n\n", result_asm == result_strspn ? "Passed" : "Failed");

    printf("Test Case: asm_strspn(\"%s\", \"%s %s\")\n", S1, A1, A2);
    result_asm = asm_strspn(S1, A1 A2);
    result_strspn = strspn(S1, A1 A2);
    printf("asm_strspn result: %zu\n", result_asm);
    printf("strspn result: %zu\n", result_strspn);
    printf("Result: %s\n\n", result_asm == result_strspn ? "Passed" : "Failed");

    printf("Test Case: asm_strspn(\"%s\", \"%s\")\n", "", A1);
    result_asm = asm_strspn("", A1);
    result_strspn = strspn("", A1);
    printf("asm_strspn result: %zu\n", result_asm);
    printf("strspn result: %zu\n", result_strspn);
    printf("Result: %s\n\n", result_asm == result_strspn ? "Passed" : "Failed");

    printf("Test Case: asm_strspn(\"%s\", \"%s\")\n", "Holberton School", "");
    result_asm = asm_strspn("Holberton School", "");
    result_strspn = strspn("Holberton School", "");
    printf("asm_strspn result: %zu\n", result_asm);
    printf("strspn result: %zu\n", result_strspn);
    printf("Result: %s\n\n", result_asm == result_strspn ? "Passed" : "Failed");

    // Test case with a long source string
    char long_string[] = "This is a very long string with various characters ,. abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    printf("Test Case: asm_strspn(long_string, \"%s\")\n", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ,.");
    result_asm = asm_strspn(long_string, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ,.");
    result_strspn = strspn(long_string, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ,.");
    printf("asm_strspn result: %zu\n", result_asm);
    printf("strspn result: %zu\n", result_strspn);
    printf("Result: %s\n\n", result_asm == result_strspn ? "Passed" : "Failed");

    printf("All tests completed.\n");

    return EXIT_SUCCESS;
}
