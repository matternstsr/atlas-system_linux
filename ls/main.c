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
	int i;
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [DIRPATH]...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (i = 1; i < argc; i++)
    {
        const char *directory_path = argv[i];
        DirectoryReader reader;

        if (mattcomp(directory_path, "-a") == 0)
        {
            continue; /* Skip to the next argument */
        }

        if (initDirectoryReader(&reader, directory_path) == -1)
        {
            fprintf(stderr, "Failure opening directory '%s'\n", directory_path);
            return EXIT_FAILURE;
        }

        printf("%s:\n", directory_path);
        if (forEachEntry(&reader, printEntryName) == -1)
        {
            fprintf(stderr, "Error occurred parsing directory '%s'\n", directory_path);
            return EXIT_FAILURE;
        }
        
        destroyDirectoryReader(&reader);
        printf("\n");
    }

    return EXIT_SUCCESS;
}
