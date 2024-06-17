#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "elf.h"

void readelf_header(const char *filename) {
    int fd;
    Elf32_Ehdr ehdr32;  // Using Elf32_Ehdr for 32-bit ELF headers

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (read(fd, &ehdr32, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (int i = 0; i < EI_NIDENT; ++i) {
        printf("%02x ", ehdr32.e_ident[i]);
    }
    printf("\n");

    printf("  Class:                             %s\n", (ehdr32.e_ident[EI_CLASS] == ELFCLASS32) ? "ELF32" : "ELF64");

    printf("  Data:                              %s\n", (ehdr32.e_ident[EI_DATA] == ELFDATA2LSB) ? "2's complement, little endian" : "2's complement, big endian");

    printf("  Version:                           %u (current)\n", (unsigned int)ehdr32.e_ident[EI_VERSION]);

    printf("  OS/ABI:                            ");
    switch (ehdr32.e_ident[EI_OSABI]) {
        case 0x00:
            printf("UNIX - System V\n");
            break;
        case 0x06:
            printf("UNIX - Solaris\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }

    printf("  ABI Version:                       %u\n", (unsigned int)ehdr32.e_ident[EI_ABIVERSION]);

    switch (ehdr32.e_type) {
        case ET_NONE:
            printf("  Type:                              NONE (Unknown file type)\n");
            break;
        case ET_REL:
            printf("  Type:                              REL (Relocatable file)\n");
            break;
        case ET_EXEC:
            printf("  Type:                              EXEC (Executable file)\n");
            break;
        case ET_DYN:
            printf("  Type:                              DYN (Shared object file)\n");
            break;
        case ET_CORE:
            printf("  Type:                              CORE (Core file)\n");
            break;
        default:
            printf("  Type:                              Unknown\n");
            break;
    }

    printf("  Machine:                           ");
    switch (ehdr32.e_machine) {
        case EM_386:
            printf("Intel 80386\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }

    printf("  Version:                           0x%x\n", (unsigned int)ehdr32.e_version);
    printf("  Entry point address:               0x%x\n", (unsigned int)ehdr32.e_entry);
    printf("  Start of program headers:          %u (bytes into file)\n", (unsigned int)ehdr32.e_phoff);
    printf("  Start of section headers:          %u (bytes into file)\n", (unsigned int)ehdr32.e_shoff);
    printf("  Flags:                             0x%x\n", (unsigned int)ehdr32.e_flags);
    printf("  Size of this header:               %u (bytes)\n", (unsigned int)ehdr32.e_ehsize);
    printf("  Size of program headers:           %u (bytes)\n", (unsigned int)ehdr32.e_phentsize);
    printf("  Number of program headers:         %u\n", (unsigned int)ehdr32.e_phnum);
    printf("  Size of section headers:           %u (bytes)\n", (unsigned int)ehdr32.e_shentsize);
    printf("  Number of section headers:         %u\n", (unsigned int)ehdr32.e_shnum);
    printf("  Section header string table index: %u\n", (unsigned int)ehdr32.e_shstrndx);
}
