#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory_reader.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


int isDirectory(const char *path) {
    struct stat statbuf;
    if (lstat(path, &statbuf) == -1) {
        return 0; /* Not a directory or doesn't exist */
    }
    return S_ISDIR(statbuf.st_mode) ? 1 : 0; /* 1 if directory, 0 otherwise */
}

int main(int argc, char **argv) {
    int i;
    struct stat statbuf;
    int has_multiple_dirs = argc > 2;
    DirectoryReader reader;
    const char *path;
    int type;
    int init_result;

    if (argc < 2) {
        /* If no directory path provided, list contents of the current directory */
        path = ".";
        type = isDirectory(path);
        if (type == 1) { /* Directory */
            if ((init_result = initDirectoryReader(&reader, path)) == -1) {
                fprintf(stderr, "%s: cannot open directory %s: %s\n", argv[0], path, matt_strerror(errno));
                return (EXIT_FAILURE);
            }

            if (forEachEntry(&reader, printEntryName) == -1) {
                fprintf(stderr, "%s: error occurred parsing directory %s: %s\n", argv[0], path, matt_strerror(errno));
                return (EXIT_FAILURE);
            }

            destroyDirectoryReader(&reader);
        } else {
            fprintf(stderr, "Usage: %s [DIRPATH]...\n", argv[0]);
            return (EXIT_FAILURE);
        }
    } else {
        /* If directory path provided, process each directory path */
        for (i = 1; i < argc; i++) {
            path = argv[i];
            type = isDirectory(path);

            if (type == 0) {
                if (lstat(path, &statbuf) == -1) {
                    fprintf(stderr, "%s: cannot access %s: %s\n", argv[0], path, matt_strerror(errno));
                    continue;
                }
                printf("%s\n", path); /* Print the path if it's not a directory */
                continue;
            }

            if (type == 1) { /* Directory */
                if ((init_result = initDirectoryReader(&reader, path)) == -1) {
                    fprintf(stderr, "%s: cannot open directory %s: %s\n", argv[0], path, matt_strerror(errno));
                    return (EXIT_FAILURE);
                }

                if (has_multiple_dirs) { /* Print directory path only if there are multiple directories */
                    printf("%s:\n", path);
                }

                if (forEachEntry(&reader, printEntryName) == -1) {
                    fprintf(stderr, "%s: error occurred parsing directory %s: %s\n", argv[0], path, matt_strerror(errno));
                    return (EXIT_FAILURE);
                }

                destroyDirectoryReader(&reader);

                if (has_multiple_dirs && i < argc - 1) /* Print new line if there are more directories */
                    printf("\n");
            }
        }
    }

    return (EXIT_SUCCESS);
}
