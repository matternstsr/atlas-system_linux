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
