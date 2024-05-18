#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory_reader.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

int isDirectory(const char *path) {
    struct stat statbuf;
    if (lstat(path, &statbuf) == -1) {
        return 0; /* Not a directory or doesn't exist */
    }
    return S_ISDIR(statbuf.st_mode) ? 1 : 0; /* 1 if directory, 0 otherwise */
}

void print_error(const char *program_name, const char *message) {
    fprintf(stderr, "%s: %s\n", program_name, message);
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
        print_error(argv[0], "Usage: [DIRPATH]...");
        return EXIT_FAILURE;
    }

    /* Process each directory path provided */
    for (i = 1; i < argc; i++) {
        path = argv[i];
        type = isDirectory(path);

        if (type == 0) {
            if (lstat(path, &statbuf) == -1) {
                char error_message[256];
                snprintf(error_message, sizeof(error_message), "cannot access %s: No such file or directory", path);
                print_error(argv[0], error_message);
                has_error = 1;
                continue;
            }
            printf("%s\n", path); /* Print the path if it's not a directory */
            continue;
        }

        if (type == 1) { /* Directory */
            if ((init_result = initDirectoryReader(&reader, path)) == -1) {
                char error_message[256];
                snprintf(error_message, sizeof(error_message), "cannot open directory %s: %s", path, strerror(errno));
                print_error(argv[0], error_message);
                has_error = 1;
                return EXIT_FAILURE;
            }

            printf("%s:\n", path);

            if (forEachEntry(&reader, printEntryName) == -1) {
                char error_message[256];
                snprintf(error_message, sizeof(error_message), "error occurred parsing directory %s: %s", path, strerror(errno));
                print_error(argv[0], error_message);
                has_error = 1;
                return EXIT_FAILURE;
            }

            destroyDirectoryReader(&reader);

            if (i < argc - 1) /* Print new line if there are more directories */
                printf("\n");
        }
    }

    fprintf(stderr, "[stderr]: [Anything]\n");

    return has_error ? EXIT_FAILURE : EXIT_SUCCESS;
}
