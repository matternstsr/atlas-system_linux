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
	unsigned char val = (unsigned char)value; /* to ensure value is treated as unsigned */
	while (num--) {
			*p++ = val;
	}
	return ptr;
}

/* Sort directory entries by name using mattsort (Bubble Sort) */
void mattsort(struct dirent **entries, int num_entries) {
    int i, j;
    for (i = 0; i < num_entries - 1; i++) {
        for (j = 0; j < num_entries - i - 1; j++) {
            if (mattcomp(entries[j]->d_name, entries[j+1]->d_name) > 0) {
                /* Swap entries */
                struct dirent *temp = entries[j];
                entries[j] = entries[j+1];
                entries[j+1] = temp;
            }
        }
    }
}
