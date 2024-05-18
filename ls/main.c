#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directory_reader.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h> /* Include errno for error handling */
#include <limits.h>

/* Custom error function */
const char *mattError(int errnum) {
    switch (errnum) {
        case ENOENT:
            return "No such file or directory";
        case EACCES:
            return "Permission denied";
        default:
            return "Unknown error";
    }
}

int main(int argc, char **argv) {
    int i;
    struct stat statbuf;
    DirectoryReader reader;
    const char *path;
    int init_result;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [DIRPATH]...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (i = 1; i < argc; i++) {
        path = argv[i];

        if (lstat(path, &statbuf) == -1) {
            fprintf(stderr, "%s: cannot access %s: %s\n", argv[0], path, strerror(errno));
            continue;
        }
        if (!S_ISDIR(statbuf.st_mode)) {
            printf("%s\n", path); /* Print the path if it's not a directory */
            continue;
        }

        if ((init_result = initDirectoryReader(&reader, path)) == -1) {
            fprintf(stderr, "%s: cannot open directory %s: %s\n", argv[0], path, strerror(errno));
            return EXIT_FAILURE;
        }

        printf("%s:\n", path); /* Print the directory path */

        if (forEachEntry(&reader, printEntryName) == -1) {
            fprintf(stderr, "%s: error parsing directory %s: Parsing error\n", argv[0], path);
            destroyDirectoryReader(&reader); /* Clean up before returning */
            return EXIT_FAILURE;
        }

        destroyDirectoryReader(&reader);

        if (i < argc - 1) /* Print new line if there are more directories */
            printf("\n");
    }

    return EXIT_SUCCESS;
}
