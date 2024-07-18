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
