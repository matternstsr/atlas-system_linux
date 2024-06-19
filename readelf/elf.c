#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "notelf.h"

/* Utility functions for endian swapping */
uint16_t swap16(uint16_t val, bool swap) {
    return swap ? ((val << 8) | (val >> 8)) : val;
}

uint32_t swap32(uint32_t val, bool swap) {
    if (swap) {
        return ((val << 24) & 0xFF000000) |
               ((val <<  8) & 0x00FF0000) |
               ((val >>  8) & 0x0000FF00) |
               ((val >> 24) & 0x000000FF);
    }
    return val;
}

/* Function to read and print ELF header for 64-bit ELF */
void readelf_header64(const char *filename, bool flip_endian) {
    int fd, i;
    Elf64_Ehdr ehdr;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (read(fd, &ehdr, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr)) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    /* Print ELF Header */
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (i = 0; i < EI_NIDENT; ++i) {
        printf("%02x ", ehdr.e_ident[i]);
    }
    printf("\n");

    printf("  Class:                             %s\n", (ehdr.e_ident[EI_CLASS] == ELFCLASS64) ? "ELF64" : "ELF32");
    printf("  Data:                              %s\n", (ehdr.e_ident[EI_DATA] == ELFDATA2LSB) ? "2's complement, little endian" : "2's complement, big endian");
    printf("  Version:                           %u (current)\n", (unsigned int)ehdr.e_ident[EI_VERSION]);

    print_osabi(ehdr.e_ident[EI_OSABI]);
    printf("  ABI Version:                       %u\n", (unsigned int)ehdr.e_ident[EI_ABIVERSION]);
    print_file_type(swap16(ehdr.e_type, flip_endian));
    print_machine_type(swap16(ehdr.e_machine, flip_endian), flip_endian); // Corrected swap16 usage

    printf("  Version:                           0x%x\n", ehdr.e_version);
    printf("  Entry point address:               0x%lx\n", (unsigned long)swap32(ehdr.e_entry, flip_endian));
    printf("  Start of program headers:          %lu (bytes into file)\n", (unsigned long)swap32(ehdr.e_phoff, flip_endian));
    printf("  Start of section headers:          %lu (bytes into file)\n", (unsigned long)swap32(ehdr.e_shoff, flip_endian));
    printf("  Flags:                             0x%x\n", swap32(ehdr.e_flags, flip_endian));
    printf("  Size of this header:               %u (bytes)\n", swap16(ehdr.e_ehsize, flip_endian));
    printf("  Size of program headers:           %u (bytes)\n", swap16(ehdr.e_phentsize, flip_endian));
    printf("  Number of program headers:         %u\n", swap16(ehdr.e_phnum, flip_endian));
    printf("  Size of section headers:           %u (bytes)\n", swap16(ehdr.e_shentsize, flip_endian));
    printf("  Number of section headers:         %u\n", swap16(ehdr.e_shnum, flip_endian));
    printf("  Section header string table index: %u\n", swap16(ehdr.e_shstrndx, flip_endian));
}

/* Function to read and print ELF header for 32-bit ELF */
void readelf_header32(const char *filename, bool flip_endian) {
    int fd, i;
    Elf32_Ehdr ehdr;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (read(fd, &ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    /* Print ELF Header */
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (i = 0; i < EI_NIDENT; ++i) {
        printf("%02x ", ehdr.e_ident[i]);
    }
    printf("\n");

    printf("  Class:                             %s\n", (ehdr.e_ident[EI_CLASS] == ELFCLASS32) ? "ELF32" : "ELF32");
    printf("  Data:                              %s\n", (ehdr.e_ident[EI_DATA] == ELFDATA2LSB) ? "2's complement, little endian" : "2's complement, big endian");
    printf("  Version:                           %u (current)\n", (unsigned int)ehdr.e_ident[EI_VERSION]);

    print_osabi(ehdr.e_ident[EI_OSABI]);
    printf("  ABI Version:                       %u\n", (unsigned int)ehdr.e_ident[EI_ABIVERSION]);
    print_file_type(swap16(ehdr.e_type, flip_endian));
    print_machine_type(swap16(ehdr.e_machine, flip_endian), flip_endian); // Corrected swap16 usage

    printf("  Version:                           0x%x\n", ehdr.e_version);
    printf("  Entry point address:               0x%lx\n", (unsigned long)swap32(ehdr.e_entry, flip_endian));
    printf("  Start of program headers:          %lu (bytes into file)\n", (unsigned long)swap32(ehdr.e_phoff, flip_endian));
    printf("  Start of section headers:          %lu (bytes into file)\n", (unsigned long)swap32(ehdr.e_shoff, flip_endian));
    printf("  Flags:                             0x%x\n", swap32(ehdr.e_flags, flip_endian));
    printf("  Size of this header:               %u (bytes)\n", swap16(ehdr.e_ehsize, flip_endian));
    printf("  Size of program headers:           %u (bytes)\n", swap16(ehdr.e_phentsize, flip_endian));
    printf("  Number of program headers:         %u\n", swap16(ehdr.e_phnum, flip_endian));
    printf("  Size of section headers:           %u (bytes)\n", swap16(ehdr.e_shentsize, flip_endian));
    printf("  Number of section headers:         %u\n", swap16(ehdr.e_shnum, flip_endian));
    printf("  Section header string table index: %u\n", swap16(ehdr.e_shstrndx, flip_endian));
}
