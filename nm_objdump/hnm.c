#include "hnm.h"
/**
* main - Entry point to process ELF files and display symbol tables.
* @argc: Argument count
* @argv: Argument vector
* return: 0 on success, 1+ on error
*/
int main(int argc, char **argv)
{
	int exit_status = EXIT_SUCCESS; /* Initialize exit status to success */
	char **file_names = argv + 1;    /* Skip program name */
	char **current_file;

	/* Check if at least one file name is provided */
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <file1> [<file2> ...]\n", argv[0]);
		return (EXIT_FAILURE); /* Rtn fail if less than 2 arguments provided */
	}
	/* Process each file provided as argument */
	for (current_file = file_names; *current_file != NULL; ++current_file)
	{
		exit_status += process_file(*current_file,
		(current_file != file_names), argv);
		/* Accumulate exit status returned by process_file for each file */
	}
	return (exit_status); /* return accumulated exit status */
}

/**
* process_file - Process an ELF file and print its symbol tables.
* @file_name: Name of the file to process
* @multiple: Flag indicating if multiple files are being processed
* @argv: Argument vector passed to main
* return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
*/
int process_file(char *file_name, int multiple, char **argv)
{
	elf_t elf_header;
	int fd, exit_status;

	fd = open_and_validate_elf(file_name, &elf_header, argv);
	if (fd == -1)
		return (EXIT_FAILURE); /* return failure if ELF file couldn't be proc */
	if (multiple)
		printf("\n%s:\n", file_name); /* Print filename if proc mult files */
	exit_status = process_and_print_symbols(&elf_header, fd, argv, file_name);
	return (exit_status); /* return exit status */
}

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
			E("File format not recognized for 32-bit ELF");
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
