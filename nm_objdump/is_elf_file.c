#include "hnm.h"

/**
* is_elf_file - Check if the buffer starts with ELF magic bytes.
* @header_buffer: Pointer to a buffer containing the ELF header.
* Return: 1 if ELF magic bytes are found, otherwise 0.
*/
int is_elf_file(char *header_buffer)
{
	int i;

	/* ELF magic bytes: 0x7f, 'E', 'L', 'F' */
	const char elf_magic[] = { 0x7f, 'E', 'L', 'F' };
	/* Check if the first 4 bytes match the ELF magic bytes */
	for (i = 0; i < 4; ++i)
	{
		if (header_buffer[i] != elf_magic[i])
			return (0);  /* Not an ELF file */
	}
	return (1);  /* ELF file detected */
}
