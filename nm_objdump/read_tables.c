#include "hnm.h"

/**
* read_string_table - reads the string table
* @elf_header: the internal header
* @fd: file descriptor to read
* return: pointer to beginning of table
*/
char *read_string_table(elf_t *elf_header, int fd)
{
	char *str;   /* Pointer to store string table */
	/* Allocate memory for the string table */
	str = calloc(1, SGET(EGET(e_shstrndx), sh_size));
	if (!str)
		exit(1);   /* Exit if allocation fails */
	/* Seek to the offset where string table starts */
	lseek(fd, SGET(EGET(e_shstrndx), sh_offset), SEEK_SET);
	/* Read the string table from file */
	read(fd, str, SGET(EGET(e_shstrndx), sh_size));
	return (str);   /* return pointer to the beginning of the table */
}

/**
* read_symbol_table - reads the symbol headers into data
* @elf_header: the internal header
* @fd: file descriptor to read
* @i: the current symbol section to print
*/
void read_symbol_table(elf_t *elf_header, int fd, int i)
{
	size_t r;
	char *headers;
	size_t read_size = SGET(i, sh_size);
	/* Allocate memory for symbol headers */
	headers = calloc(1, read_size);
	if (!headers)
		exit(1);  /* Exit if memory allocation fails */
	/* Seek to the offset of the symbol section and read data */
	lseek(fd, SGET(i, sh_offset), SEEK_SET);
	r = read(fd, headers, read_size);
	/* Check if read operation was successful */
	if (r < read_size)
	{
		fprintf(stderr, "Symbol read failed.\n");
		exit(1);  /* Exit if read operation failed */
	}
	/* Assign read data to the right member in elf_header based on ELF class */
	if (IS_32(elf_header->e64))
		elf_header->y32 = (void *)headers;
	else
		elf_header->y64 = (void *)headers;
}


/**
* read_symbol_string_table - reads the symbol string table
* @elf_header: the internal header
* @fd: file descriptor to read
* @i: the current symbol section index
* return: pointer to the beginning of the symbol string table
*/
char *read_symbol_string_table(elf_t *elf_header, int fd, int i)
{
	char *str;

	/* Allocate memory for the symbol string table */
	str = calloc(1, SGET(i, sh_size));
	if (!str)
		exit(1);  /* Exit if memory allocation fails */
	/* Seek to the offset of the symbol string table and read data */
	lseek(fd, SGET(i, sh_offset), SEEK_SET);
	read(fd, str, SGET(i, sh_size));
	return (str);  /* return a ptr to the beginning of the symbol string table */
}


size_t find_verneed_index(Elf64_Verneed *verneed, size_t verneed_size,
						size_t index)
{
	size_t i = 0, ret = 0;

	/* Iterate through the verneed array */
	for (i = 1; i < verneed_size; i++)
	{
		/* If vn_file field is out of bounds, return 0 */
		if (verneed[i].vn_file >= verneed_size)
			return (0);
		/* If vn_file matches the index, update the return value */
		if (verneed[i].vn_file == index)
			ret = i;
	}
	return (ret);  /* return the index of verneed entry, or 0 if not found */
}
