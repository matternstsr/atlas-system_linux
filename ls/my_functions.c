/* my_functions.c */
#include "directory_reader.h"

/* Custom memset function */
void *mattset(void *ptr, int value, size_t num)
{
unsigned char *p = ptr;
/* to ensure value is treated as unsigned */
unsigned char val = (unsigned char)value;

while (num--)
*p++ = val;
return (ptr);
}

/* Custom string comparison function */
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
