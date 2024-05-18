/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory_reader.h"

/**
 * main - Entry point of the program.
 * @argc: Number of command-line arguments.
 * @argv: Array of command-line argument strings.
 *
 * Description: Entry point of the program. It processes
 * command-line arguments and initiates directory reading.
 *
 * Return: EXIT_SUCCESS if successful, EXIT_FAILURE if an error occurs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory_reader.h"

int main(int argc, char **argv)
{
	DirectoryReader reader;
	int i;
	const char *path = argv[i];
	int is_dir = isDirectory(path);
	int init_result;
	int has_multiple_dirs = argc > 2 && i < argc - 1;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [DIRPATH]...\n", argv[0]);
		return (EXIT_FAILURE);
	}
	for (i = 1; i < argc; i++)
	{
		if (is_dir)
		{
			if ((init_result = initDirectoryReader(&reader, path)) == -1)
			{
				fprintf(stderr, "Failure opening directory '%s'\n", path);
				return (EXIT_FAILURE);
			}
			if (has_multiple_dirs) /* Print directory path only if there are multiple directories */
				printf("%s:\n", path);
			if (forEachEntry(&reader, printEntryName) == -1)
			{
				fprintf(stderr, "Error occurred parsing directory '%s'\n", path);
				return (EXIT_FAILURE);
			}
			destroyDirectoryReader(&reader);
			if (has_multiple_dirs) /* Print new line if there are multiple directories */
				printf("\n");
		}
		else
		{
			/* Print the file path */
			printf("%s\n", path);
		}
	}
	return (EXIT_SUCCESS);
}