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
 * Return: Pointer to the memory area.
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
 * Return: An integer less than, equal to,
 *          or greater than zero if str1 is found, respectively,
 *          to be less than, to match, or be greater than str2.
 */
int mattcomp(const char *str1, const char *str2)
{
while (*str1 && *str2)
{
char c1 = mattlower(*str1); /* Convert chars to lowercase for comp */
char c2 = mattlower(*str2);
if (c1 != c2)
	return (c1 - c2);
str1++;
str2++;
}
return (*str1 - *str2);
}

/**
 * mattlower - Converts a character to lowercase.
 * @c: The character to convert to lowercase.
 *
 * Description: This function converts the given character to lowercase.
 *
 * Return: The lowercase version of the character.
 */
char mattlower(char c)
{
if (c >= 'A' && c <= 'Z')
	/* Convert uppercase letter to lowercase */
	return (c + ('a' - 'A'));
else
	/* Return unchanged for non-uppercase characters */
	return (c);
}

void mattsort(struct dirent **entries, int num_entries)
{
	int i, j;
	struct dirent *temp;

	for (i = 0; i < num_entries - 1; i++)
	{
		for (j = 0; j < num_entries - i - 1; j++)
		{
			/* Compare directory entry names and swap if necessary */
			if (mattcomp(entries[j]->d_name, entries[j + 1]->d_name) > 0)
			{
				temp = entries[j];
				entries[j] = entries[j + 1];
				entries[j + 1] = temp;
			}
		}
	}
}
