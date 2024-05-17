/* my_functions.c */
#include "directory_reader.h"

/* Custom string comparison function */
int mattcomp(const char *str1, const char *str2) {
		while (*str1 && *str2) {
				if (*str1 != *str2)
						return *str1 - *str2;
				str1++;
				str2++;
		}
		return *str1 - *str2;
}

/* Custom memset function */
void *mattset(void *ptr, int value, size_t num) {
	unsigned char *p = ptr;
	unsigned char val = (unsigned char)value; // Ensure value is treated as unsigned
	while (num--) {
			*p++ = val;
	}
	return ptr;
}