/* main.c */

#include "directory_reader.h"

/* Comparison function for qsort */
int compareEntries(const void *a, const void *b) {
	return mattcomp((*(struct dirent **)a)->d_name, (*(struct dirent **)b)->d_name);
}

int main(int argc, char **argv) {
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
			if (mattcomp(argv[1], "-a") == 0) {
					show_all = 0;
					directory_path = ".";
			}
	} else {
			directory_path = "."; /* Default to current directory if no directory path provided */
	}

	/* Initialize directory reader */
	if (initDirectoryReader(&reader, directory_path) == -1) {
			fprintf(stderr, "Failure opening directory '%s'\n", directory_path);
			return EXIT_FAILURE;
	}

	reader.show_all = show_all;

	/* Collect directory entries */
	int entry_count = 0;
	while (getNextEntry(&reader)) {
			entries[entry_count++] = reader.current_entry;
	}

	/* Sort entries alphabetically */
	qsort(entries, entry_count, sizeof(struct dirent *), compareEntries);

	/* Print sorted directory entries */
	for (int i = 0; i < entry_count; ++i) {
			printEntryName(entries[i]);
	}

	/* Cleanup */
	destroyDirectoryReader(&reader);
	return EXIT_SUCCESS;
}
