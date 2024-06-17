// atlas-system_linux/readelf/0-hreadelf.c

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> // defining the consts like EXIT_FAILURE and EXIT_SUCCESS
#include "notelf.h"  // Include elf.h for ELF header definitions

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ELF file>\n", argv[0]);
        return(EXIT_FAILURE);
    }

    readelf_header(argv[1]);

    return(EXIT_SUCCESS);
}

