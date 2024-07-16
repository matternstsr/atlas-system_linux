#include "hnm.h"

/**
* read_section_headers - reads the section headers into data
* @elf_header: the internal header
* @fd: file descriptor to read
*/
void read_section_headers(elf_t *elf_header, int fd)
{
	size_t i = EGET(e_shnum), r;
	/* Initialize i with the number of section headers */
	char *headers;                  /* Pointer to store section headers */
	size_t read_size = EGET(e_shentsize) * EGET(e_shnum);
	/* Calculate total size to read */
	/* If there are no section headers, return */
	if (!i)
		return;
	/* Allocate memory to store section headers */
	headers = calloc(i, read_size);
	if (!headers)
		exit(1);   /* Exit if allocation fails */
	/* Seek to the offset where section headers start */
	lseek(fd, EGET(e_shoff), SEEK_SET);
	/* Read section headers from file */
	r = read(fd, headers, read_size);
	/* If read size doesn't match expected size, exit (TODO: handle better) */
	if (r < read_size)
		exit(1);   /* Exit if read fails */
	/* Assign read hdrs to the appropriate struct field based on ELF type */
	if (IS_32(elf_header->e64))
		elf_header->s32 = (void *)headers;
	else
		elf_header->s64 = (void *)headers;
}

/**
* read_data - reads a block of data from descriptor
* @elf_header: the internal header (unused)
* @fd: the descriptor to read from
* @offset: offset to start reading from
* @size: number of bytes to read
* return: pointer to the allocated data block
*/
void *read_data(elf_t *elf_header, int fd, unsigned long offset, long size)
{
	void *str = NULL;

	/* Allocate memory for the data block */
	str = calloc(1, size);
	if (!str)
		exit(1);  /* Exit if allocation fails */
	/* Seek to the specified offset in the file */
	lseek(fd, offset, SEEK_SET);
	/* Read 'size' bytes of data from the file descriptor 'fd' */
	if (read(fd, str, size) != size)
	{
		fprintf(stderr, "Read error.\n");
		exit(1);  /* Exit if read fails */
	}
	return (str);  /* return the pointer to the allocated data block */
	(void)elf_header;  /* Unused parameter (suppress compiler warning) */
}
