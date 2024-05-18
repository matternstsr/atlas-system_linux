/* my_functions.c */
#include "directory_reader.h"

/**
 * mattset - Custom memset function.
 * @ptr: Pointer to the memory area to be filled.
 * @value: Value to be set. Note: treated as unsigned.
 * @num: Number of bytes to be set to the value.
 *
 * Description: This function fills the memory area pointed to
 * by ptr with the specified value.
 *
 * Returns: Pointer to the memory area.
 */
void *mattset(void *ptr, int value, size_t num)
{
unsigned char *p = ptr;
/* to ensure value is treated as unsigned */
unsigned char val = (unsigned char)value;

while (num--)
*p++ = val;
return (ptr);
}

/**
 * mattcomp - Custom string comparison function.
 * @str1: Pointer to the first string.
 * @str2: Pointer to the second string.
 *
 * Description: This function compares two strings case-insensitively.
 *
 * Returns: An integer less than, equal to,
 *          or greater than zero if str1 is found, respectively,
 *          to be less than, to match, or be greater than str2.
 */
int mattcomp(const char *str1, const char *str2)
{
while (*str1 && *str2)
{
char c1 = tolower(*str1); /* Convert characters to lowercase for comparison */
char c2 = tolower(*str2);
if (c1 != c2)
	return (c1 - c2);
str1++;
str2++;
}
return (*str1 - *str2);
}
