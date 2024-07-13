#include "elf_parser.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "[stderr]: Usage: %s [objfile...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; ++i)
    {
        const char *filename = argv[i];
        if (process_file(filename) != 0)
        {
            fprintf(stderr, "[stderr]: Error processing file: %s\n", filename);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

