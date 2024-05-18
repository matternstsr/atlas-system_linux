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


int main(int argc, char **argv) {
    int i;
    struct stat statbuf;
    int has_multiple_dirs = argc > 2;
    DirectoryReader reader;
    const char *path;
    int init_result;

    if (argc < 2) {
        printf("Usage: %s [DIRPATH]...\n", argv[0]);
        return (EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++) {
        path = argv[i];

        // Skip the program name itself
        if (i == 0) {
            continue;
        }

        if (lstat(path, &statbuf) == -1) {
            printf("%s: cannot access %s: %s\n", argv[0], path, mattError(errno));
            continue;
        }
        if (!S_ISDIR(statbuf.st_mode)) {
            printf("%s\n", path); /* Print the path if it's not a directory */
            continue;
        }

        if ((init_result = initDirectoryReader(&reader, path)) == -1) {
            printf("%s: cannot open directory %s: %s\n", argv[0], path, mattError(errno));
            return (EXIT_FAILURE);
        }

        if (has_multiple_dirs) { /* Print directory path only if there are multiple directories */
            printf("%s:\n", path);
        }

        if (forEachEntry(&reader, printEntryName) == -1) {
            printf("%s: error parsing directory %s: Parsing error\n", argv[0], path);
            return (EXIT_FAILURE);
        }

        destroyDirectoryReader(&reader);

        if (has_multiple_dirs && i < argc - 1) /* Print new line if there are more directories */
            printf("\n");
    }

    return (EXIT_SUCCESS);
}
