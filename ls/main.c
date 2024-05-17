/* main.c */

#include "directory_reader.h"

int main(int argc, char **argv) {

	int entry_count, i;

    const char *directory_path;
    int show_all = 1;
    DirectoryReader reader;
    struct dirent *entries[1000]; /* Assuming max 1000 entries */

    /* Check for correct usage */
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [DIRPATH]\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Set directory path */
    if (argc == 2) {
        directory_path = argv[1];
        /* Check if the user wants to show hidden files */
        if (strcmp(argv[1], "-a") == 0) {
            show_all = 0;
            directory_path = ".";
        }
    } else {
        directory_path = "."; /*  Default to current directory if no directory path provided */
    }

    /* Initialize directory reader */
    if (initDirectoryReader(&reader, directory_path) == -1) {
        fprintf(stderr, "Failure opening directory '%s'\n", directory_path);
        return EXIT_FAILURE;
    }

    reader.show_all = show_all;

    /* Collect directory entries */
    entry_count = 0;
    while (getNextEntry(&reader)) {
        entries[entry_count++] = reader.current_entry;
    }

    /* Sort entries alphabetically */
    qsort(entries, entry_count, sizeof(struct dirent *), compareEntries);

    /* Print sorted directory entries */
    for (i = 0; i < entry_count; ++i) {
        printEntryName(entries[i]);
    }

    /* Cleanup */
    destroyDirectoryReader(&reader);
    return EXIT_SUCCESS;
}