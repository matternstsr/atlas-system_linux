#include "hnm.h"

/**
* main - Entry point to process ELF files and display symbol tables.
* @argc: Argument count
* @argv: Argument vector
* return: 0 on success, 1+ on error
*/
int main(int ac, char **argv)
{
	int retstatus = EXIT_SUCCESS;/* Initialize return status */
	char **_argv = argv;/* Pointer to iterate through arguments */

	if (ac < 2)
		return (fprintf(stderr, USAGE), EXIT_FAILURE);/*Print use on insu args*/
	if (ac == 2)
		return (process_file(argv[1], 0, argv));/*Proc single file only 1 arg*/
	/* Process multiple files */
	while (*++_argv)
		retstatus += process_file(*_argv, 1, argv);
	return (retstatus);/* Return cumulative status */
}

