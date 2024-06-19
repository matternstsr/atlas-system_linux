/* atlas-system_linux/readelf/0-hreadelf.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "notelf.h"

int main(int argc, char *argv[]) {
    int fd;
    unsigned char ident[EI_NIDENT];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ELF file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Open ELF file */
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    /* Read ELF header identification bytes */
    if (read(fd, ident, EI_NIDENT) != EI_NIDENT) {
        perror("read");
        close(fd);
        return EXIT_FAILURE;
    }

    /* Close file */
    close(fd);

    /* Determine and print ELF header information */
    if (ident[EI_CLASS] == ELFCLASS64) {
        readelf_header64(argv[1], ident[EI_DATA] == ELFDATA2MSB);
    } else if (ident[EI_CLASS] == ELFCLASS32) {
        readelf_header32(argv[1], ident[EI_DATA] == ELFDATA2MSB);
    } else {
        fprintf(stderr, "Unknown ELF class\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
