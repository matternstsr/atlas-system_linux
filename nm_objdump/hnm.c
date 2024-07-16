#include "hnm.h" /* Include the header file hnm.h */

/**
* main - Entry point to process ELF files and display symbol tables.
* @argc: Argument count
* @argv: Argument vector
* Return: 0 on success, 1+ on error
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
	return (exit_status); /* Return accumulated exit status */
}

/**
* process_file - Process an ELF file and print its symbol tables.
* @file_name: Name of the file to process
* @multiple: Flag indicating if multiple files are being processed
* @argv: Argument vector passed to main
* Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
*/
int process_file(char *file_name, int multiple, char **argv)
{
	int fd, exit_status = EXIT_SUCCESS;
	size_t r, num_printed = 0;
	elf_t elf_header;

	memset(&elf_header, 0, sizeof(elf_header)); /* Initialize elf_header to zero */
	fd = crack_open_file(file_name, 0, argv); /* Open the file */
	if (fd == -1)
	{
		return (EXIT_FAILURE); /* Return failure if file couldn't be opened */
	}
	r = read(fd, &elf_header.e64, sizeof(elf_header.e64)); /* Read ELF header */
	if (r != sizeof(elf_header.e64) || !is_elf_file((char *)&elf_header.e64))
	{
		fprintf(stderr, "%s: %s: File format not recognized\n", argv[0], file_name);
		close(fd);
		return (EXIT_FAILURE); /* Return failure if ELF header is not valid */
	}
	if (IS_32(elf_header.e64))
	{ /* Check if ELF file is 32-bit */
		lseek(fd, 0, SEEK_SET); /* Rewind file pointer */
		r = read(fd, &elf_header.e32, sizeof(elf_header.e32)); /* Read 32-bit ELF header */
		if (r != sizeof(elf_header.e32) || !is_elf_file((char *)&elf_header.e32))
		{
			fprintf(stderr, "%s: %s: File format not recognized for 32-bit ELF\n", argv[0], file_name);
			close(fd);
			return (EXIT_FAILURE); /* Return failure if 32-bit ELF header is not valid */
		}
	}
	if (multiple)
	{
		printf("\n%s:\n", file_name); /* Print file name if processing multiple files */
	}
	swap_all_endian(&elf_header); /* Swap endianness of ELF header */
	exit_status = print_all_symbol_tables(&elf_header, fd, &num_printed); /* Print symbol tables */
	if (exit_status != EXIT_SUCCESS)
	{
		fprintf(stderr, "%s: %s: failed to print symbol tables\n", argv[0], file_name);
	} else if (num_printed == 0)
	{
		fprintf(stderr, "%s: %s: no symbols\n", argv[0], file_name);/*took found out of this print*/
	}
	free(elf_header.s32); /* Free allocated memory */
	free(elf_header.s64);
	free(elf_header.p32);
	free(elf_header.p64);
	close(fd); /* Close the file */
	return (exit_status); /* Return exit status */
}
