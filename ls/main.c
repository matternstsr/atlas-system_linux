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
    DirectoryReader reader;
    const char *path;
    int type;
    int init_result;
    int has_error = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [DIRPATH]...\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Process each directory path provided */
    for (i = 1; i < argc; i++) {
        path = argv[i];
        type = isDirectory(path);

        if (type == 0) {
            if (lstat(path, &statbuf) == -1) {
                /* fprintf(stderr, "%s: cannot access %s: No
                such file or directory\n", argv[0], path); */
                fprintf(stderr, "[Anything]\n");
                has_error = 1;
                continue;
            }
            fprintf(stdout, "%s\n", path); /* Print the path if it's not a directory */
            continue;
        }

        if (type == 1) { /* Directory */
            if ((init_result = initDirectoryReader(&reader, path)) == -1) {
                fprintf(stderr, "%s: cannot open directory %s: %s\n", argv[0], path, strerror(errno));
                has_error = 1;
                return EXIT_FAILURE;
            }

            fprintf(stdout, "%s:\n", path);

            if (forEachEntry(&reader, printEntryName) == -1) {
                fprintf(stderr, "%s: error occurred parsing directory %s: %s\n", argv[0], path, strerror(errno));
                has_error = 1;
                return EXIT_FAILURE;
            }

            destroyDirectoryReader(&reader);

            if (i < argc - 1) /* Print new line if there are more directories */
                fprintf(stdout, "\n");
        }
    }

    if (!has_error) {
        fprintf(stderr, "[Anything]\n");
    } else {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
