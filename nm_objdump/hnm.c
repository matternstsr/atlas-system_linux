#include <stdio.h>
#include <stdlib.h>
#include "elf_parser.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [objfile...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; ++i) {
        const char *filename = argv[i];
        printf("%s:\n", filename);
        if (process_file(filename) != 0) {
            fprintf(stderr, "Error processing file: %s\n", filename);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}