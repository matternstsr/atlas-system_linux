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
    int numDirectories = 0; /* Counter for the number of directories */
    int numEntries = 0; /* Counter for the number of entries in the directory */

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [DIRPATH]...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (i = 1; i < argc; i++) {
        path = argv[i];
        /* Increase the number of directories processed */
        numDirectories++;
        numEntries = 0; /* Reset the counter for the number of entries in the directory */

        if (lstat(path, &statbuf) == -1) {
            /* fprintf(stderr, "%s: cannot access %s: %s\n", argv[0], path, mattError(errno)); */
            continue;
        }
        if (!S_ISDIR(statbuf.st_mode)) {
            printf("%s\n", path); /* Print the path if it's not a directory */
            continue;
        }
        if ((init_result = initDirectoryReader(&reader, path)) == -1) {
            fprintf(stderr, "%s: cannot open directory %s: %s\n", argv[0], path, mattError(errno));
            continue; /* Continue to next directory instead of returning immediately */
        }

        /* Check if there are entries in the directory */
        if (forEachEntry(&reader, countEntries) == -1) {
            fprintf(stderr, "%s: error parsing directory %s: Parsing error\n", argv[0], path);
            destroyDirectoryReader(&reader); /* Clean up before continuing */
            continue; /* Continue to next directory */
        }

        if (numDirectories > 1 || numEntries > 0) {
            printf("\n%s:\n", path); /* Print the directory path if there are multiple directories or entries */
        }

        numEntries = 0; /* Reset the counter for the number of entries in the directory */

        if (forEachEntry(&reader, printEntryName) == -1) {
            fprintf(stderr, "%s: error parsing directory %s: Parsing error\n", argv[0], path);
            destroyDirectoryReader(&reader); /* Clean up before continuing */
            continue; /* Continue to next directory */
        }

        destroyDirectoryReader(&reader);

        if (i < argc - 1) /* Print new line if there are more directories */
            printf("\n");
    }

    return EXIT_SUCCESS;
}
