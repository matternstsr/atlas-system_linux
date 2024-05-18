/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory_reader.h"
#include <sys/stat.h>

int isDirectory(const char *path) {
    DIR *dir = opendir(path);

    if (dir != NULL) {
        closedir(dir);
        return (1); /* Directory exists */
    }
    return (0); /* Not a directory or doesn't exist */
}

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
    DirectoryReader reader;
    const char *path;
    int type;
    int init_result;
    int has_multiple_dirs;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [DIRPATH]...\n", argv[0]);
        return (EXIT_FAILURE);
    }
    for (i = 1; i < argc; i++)
    {
        path = argv[i];
        type = isDirectory(path);
        has_multiple_dirs = argc > 2 && i < argc - 1;

        if (type == 0)
        {
            fprintf(stderr, "%s: cannot access %s: No such file or directory\n", argv[0], path);
            continue;
        }

        if (type == 1) /* Directory */
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
        else /* File */
        {
            /* Print the file path */
            printf("%s\n", path);
        }
    }

    return (EXIT_SUCCESS);
}
