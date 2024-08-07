#include "hnm.h"

/**
* open_and_validate_elf - Open and validate an ELF file.
* @file_name: Name of the file to process
* @elf_header: Pointer to the elf_t structure to fill with ELF information
* @argv: Argument vector passed to main
* return: File descriptor on success, -1 on failure
*/
int open_and_validate_elf(char *file_name, elf_t *elf_header, char **argv)
{
	int fd;
	size_t r;
	const char *fString = "%s: %s: File format not recognized for 32-bit ELF\n";

	memset(elf_header, 0, sizeof(elf_t)); /* Init elf_header to zero */

	fd = crack_open_file(file_name, 0, argv); /* Open the file */
	if (fd == -1)
		return (-1); /* return failure if file couldn't be opened */
	r = read(fd, &elf_header->e64, sizeof(elf_header->e64)); /* ReadELFheader */
	if (r != sizeof(elf_header->e64) || !is_elf_file((char *)&elf_header->e64))
	{
		fprintf(stderr, "%s: %s: File format not recognized\n",
				argv[0], file_name);
		close(fd);
		return (-1); /* return failure if ELF header is not valid */
	}
	if (IS_32(elf_header->e64))
	{ /* Check if ELF file is 32-bit */
		lseek(fd, 0, SEEK_SET); /* Rewind file pointer */
		r = read(fd, &elf_header->e32, sizeof(elf_header->e32)); /*R32 ELF hdr*/
		if (r != sizeof(elf_header->e32) ||
						!is_elf_file((char *)&elf_header->e32))
		{
			fprintf(stderr, fString, argv[0], file_name);
			close(fd);
			return (-1); /* return failure if 32-bit ELF header is not valid */
		}
	}
	return (fd); /* return file descriptor on success */
}

/**
* process_and_print_symbols - Process an ELF file and print its symbol tables.
* @elf_header: Pointer to the elf_t structure containing ELF information
* @fd: File descriptor of the opened ELF file
* @argv: Argument vector passed to main
* @file_name: Name of the file to process
* return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
*/
int process_and_print_symbols(elf_t *elf_header, int fd, char **argv,
							char *file_name)
{
	int exit_status = EXIT_SUCCESS;
	size_t num_printed = 0;

	swap_all_endian(elf_header); /* Swap endianness of ELF header */

	exit_status = print_all_symbol_tables(elf_header, fd, &num_printed);
	/*Print symbol tables */
	if (exit_status != EXIT_SUCCESS)
		fprintf(stderr, "%s: %s: failed to print symbol tables\n",
				argv[0], file_name);
	else if (num_printed == 0)
		fprintf(stderr, "%s: %s: no symbols\n", argv[0], file_name);

	free(elf_header->s32); /* Free allocated memory */
	free(elf_header->s64);
	free(elf_header->p32);
	free(elf_header->p64);

	close(fd); /* Close the file */
	return (exit_status); /* return exit status */
}
