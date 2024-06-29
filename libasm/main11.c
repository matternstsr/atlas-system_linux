#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Declaration of assembly function
extern size_t asm_strcspn(const char *s, const char *reject);

#define S1  "Holberton"
#define S2  "holberton"
#define S3  "HOLBERTON"

#define R1  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define R2  "abcdefghijklmnopqrstuvwxyzABCDEFGIJKLMNOPQRSTUVWXYZ"
#define R3  "0123456789"
#define R4  " "
#define R5  "adfgijkmpqsuvwxyzABCDEFGIJKLMNOPQRTUVWXYZ"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    // Test cases with print statements
    printf("Test Case: asm_strcspn(\"%s\", \"%s\")\n", S1, R1);
    size_t result_asm = asm_strcspn(S1, R1);
    size_t result_strcspn = strcspn(S1, R1);
    printf("asm_strcspn result: %zu\n", result_asm);
    printf("strcspn result: %zu\n", result_strcspn);
    printf("Result: %s\n\n", result_asm == result_strcspn ? "Passed" : "Failed");

    printf("Test Case: asm_strcspn(\"%s\", \"%s\")\n", S1, R2);
    result_asm = asm_strcspn(S1, R2);
    result_strcspn = strcspn(S1, R2);
    printf("asm_strcspn result: %zu\n", result_asm);
    printf("strcspn result: %zu\n", result_strcspn);
    printf("Result: %s\n\n", result_asm == result_strcspn ? "Passed" : "Failed");

    printf("Test Case: asm_strcspn(\"%s\", \"%s\")\n", S1, R3);
    result_asm = asm_strcspn(S1, R3);
    result_strcspn = strcspn(S1, R3);
    printf("asm_strcspn result: %zu\n", result_asm);
    printf("strcspn result: %zu\n", result_strcspn);
    printf("Result: %s\n\n", result_asm == result_strcspn ? "Passed" : "Failed");

    printf("Test Case: asm_strcspn(\"%s\", \"%s\")\n", S1, R4);
    result_asm = asm_strcspn(S1, R4);
    result_strcspn = strcspn(S1, R4);
    printf("asm_strcspn result: %zu\n", result_asm);
    printf("strcspn result: %zu\n", result_strcspn);
    printf("Result: %s\n\n", result_asm == result_strcspn ? "Passed" : "Failed");

    printf("Test Case: asm_strcspn(\"%s\", \"%s\")\n", S1, R5);
    result_asm = asm_strcspn(S1, R5);
    result_strcspn = strcspn(S1, R5);
    printf("asm_strcspn result: %zu\n", result_asm);
    printf("strcspn result: %zu\n", result_strcspn);
    printf("Result: %s\n\n", result_asm == result_strcspn ? "Passed" : "Failed");

    printf("Test Case: asm_strcspn(\"%s\", \"%s\")\n", S1, "");
    result_asm = asm_strcspn(S1, "");
    result_strcspn = strcspn(S1, "");
    printf("asm_strcspn result: %zu\n", result_asm);
    printf("strcspn result: %zu\n", result_strcspn);
    printf("Result: %s\n\n", result_asm == result_strcspn ? "Passed" : "Failed");

    printf("Test Case: asm_strcspn(\"%s\", \"%s\")\n", "", R1);
    result_asm = asm_strcspn("", R1);
    result_strcspn = strcspn("", R1);
    printf("asm_strcspn result: %zu\n", result_asm);
    printf("strcspn result: %zu\n", result_strcspn);
    printf("Result: %s\n\n", result_asm == result_strcspn ? "Passed" : "Failed");

    // Test case with a long source string
    char long_string[] = "This is a very long string with various characters ,. abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    printf("Test Case: asm_strcspn(long_string, \"%s\")\n", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ,.");
    result_asm = asm_strcspn(long_string, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ,.");
    result_strcspn = strcspn(long_string, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ,.");
    printf("asm_strcspn result: %zu\n", result_asm);
    printf("strcspn result: %zu\n", result_strcspn);
    printf("Result: %s\n\n", result_asm == result_strcspn ? "Passed" : "Failed");

    printf("All tests completed.\n");

    return EXIT_SUCCESS;
}
