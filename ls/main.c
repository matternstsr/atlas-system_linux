/* main.c */

#include "directory_reader.h"
#include <string.h>

/**
 * main - Entry point of the program.
 * @argc: Number of command-line arguments.
 * @argv: Array of command-line argument strings.
 *
 * Description: Entry point of the program. It processes
 * command-line arguments and initiates directory reading.
 *
 * Returns: EXIT_SUCCESS if successful, EXIT_FAILURE if an error occurs.
 */
int main(int argc, char **argv)
{
	const char *directory_path;
	int show_all = 1;
	DirectoryReader reader;

	/* Check for correct usage */
	if (argc > 2)
	{
		fprintf(stderr, "Usage: %s [DIRPATH]\n", argv[0]);
		return (EXIT_FAILURE);
	}
	/* Set directory path */
	if (argc == 2)
	{
		directory_path = argv[1];
		/*  Check if the user wants to show hidden files */
		if (mattcomp(argv[1], "-a") == 0)
		{
		show_all = 0;
		directory_path = ".";
		}
	}
	else
		directory_path = "."; /* Default to cur dir if no dir path prov */
	/* Initialize directory reader */
	if (initDirectoryReader(&reader, directory_path) == -1)
	{
	fprintf(stderr, "Failure opening directory '%s'\n", directory_path);
	return (EXIT_FAILURE);
	}
	reader.show_all = show_all;
	/* Iterate through directory entries and print them */
	if (forEachEntry(&reader, printEntryName) == -1)
	{
	fprintf(stderr, "Error occurred parsing directory '%s'\n", directory_path);
	return (EXIT_FAILURE);
	}
	/* Cleanup */
	destroyDirectoryReader(&reader);
	return (EXIT_SUCCESS);
}
