#include "notelf.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ELF file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    readelf_header(argv[1]);

    return EXIT_SUCCESS;
}
