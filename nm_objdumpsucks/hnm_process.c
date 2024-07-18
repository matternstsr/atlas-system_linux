#include "hnm.h"

/**
* process_file - Process an ELF file and print its symbol tables.
* @file_name: Name of the file to process
* @multiple: Flag indicating if multiple files are being processed
* @argv: Argument vector passed to main
* return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
*/
int process_files(char *file_name, int multiple, char **argv)
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
