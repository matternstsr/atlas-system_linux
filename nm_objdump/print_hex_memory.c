#include "hnm.h"

/**
* print_hex_memory - prints hexadecimal representation of memory bytes
* @memory_start: starting address of the memory region
* @byte_count: number of bytes to print
*/
void print_hex_memory(void *memory_start, size_t byte_count)
{
	unsigned char *byte_ptr = memory_start; /* Pointer to the start of memory region */

	while (byte_count--) /* Loop through each byte */
		printf("%2.2hx ", *byte_ptr++); /* Print each byte in hexadecimal format */
	printf("\n"); /* Print newline at the end */
}
