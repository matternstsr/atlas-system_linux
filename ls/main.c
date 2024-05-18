/* main.c */

#include "directory_reader.h"


int isDirectory(const char *path) {
    struct stat statbuf;
    if (lstat(path, &statbuf) == -1) {
        return (0); /* Not a dir or doesn't exist */
    }
    return (S_ISDIR(statbuf.st_mode) ? 1 : 0); /* 1 if dir, 0 otherwise */
}

void printDirectoryContents(const char *dir_path) {
    DirectoryReader reader;
    int init_result;

    if ((init_result = initDirectoryReader(&reader, dir_path)) == -1) {
        fprintf(stderr, "Failure opening directory '%s'\n", dir_path);
        return;
    }

    printf("%s:\n", dir_path);
    if (forEachEntry(&reader, printEntryName) == -1) {
        fprintf(stderr, "Error occurred parsing directory '%s'\n", dir_path);
        return;
    }

    destroyDirectoryReader(&reader);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [DIRPATH]...\n", argv[0]);
        return (EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        const char *path = argv[i];
        int type = isDirectory(path);

        if (type == 0) {
            fprintf(stderr, "%s: cannot access %s: "
                "No such file or directory\n", argv[0], path);
        } else {
            printDirectoryContents(path);
        }
    }

    return (EXIT_SUCCESS);
}
