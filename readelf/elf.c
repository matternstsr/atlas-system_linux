/* elf.c */

#include "myelf.h"

/* Open ELF file and return file descriptor */
int open_elf(const char *filename) {
    int fd = open(filename, O_RDONLY); /* Open file for reading */
    if (fd == -1) { /* Check for error */
        perror("open");
        exit(EXIT_FAILURE);
    }
    return fd;
}

/* Read ELF header from file descriptor */
void read_elf_header(int fd, MyElf32_Ehdr *ehdr) {
    if (read(fd, ehdr, sizeof(MyElf32_Ehdr)) != sizeof(MyElf32_Ehdr)) {
      /* Read ELF header  */
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd); /* Close file descriptor */
}
