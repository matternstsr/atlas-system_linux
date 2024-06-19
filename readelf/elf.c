/* atlas-system_linux/readelf/elf.c */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Defined missing constants that not available in <elf.h> */
#ifndef ELFOSABI_SORTIX
#define ELFOSABI_SORTIX 9
#endif

#ifndef EM_SPARC32PLUS
#define EM_SPARC32PLUS 18
#endif

#ifndef EM_SPARCV9
#define EM_SPARCV9 43
#endif

void readelf_header(const char *filename) {
    int fd, i;
    Elf32_Ehdr ehdr32;
    
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
    for (i = 0; i < EI_NIDENT; ++i) {
        printf("%02x ", ehdr32.e_ident[i]);
    }
    printf("\n");

    printf("  Class:                             %s\n", (ehdr32.e_ident[EI_CLASS] == ELFCLASS64) ? "ELF64" : "ELF32");

    printf("  Data:                              %s\n", (ehdr32.e_ident[EI_DATA] == ELFDATA2LSB) ? "2's complement, little endian" : "2's complement, big endian");
    printf("  Version:                           %u (current)\n", (unsigned int)ehdr32.e_ident[EI_VERSION]);

    printf("  OS/ABI:                            ");
    switch (ehdr32.e_ident[EI_OSABI]) {
        case ELFOSABI_SYSV:
            printf("UNIX - System V\n");
            break;
        case ELFOSABI_HPUX:
            printf("HP-UX\n");
            break;
        case ELFOSABI_NETBSD:
            printf("NetBSD\n");
            break;
        case ELFOSABI_LINUX:
            printf("Linux\n");
            break;
        case ELFOSABI_SOLARIS:
            printf("UNIX - Solaris\n");
            break;
        case ELFOSABI_SORTIX:
            printf("Sortix\n");
            break;
        default:
            printf("<unknown: %x>\n", ehdr32.e_ident[EI_OSABI]);
            break;
    }

    printf("  ABI Version:                       %u\n", (unsigned int)ehdr32.e_ident[EI_ABIVERSION]);

    printf("  Type:                              ");
    switch (ehdr32.e_type) {
        case ET_NONE:
            printf("NONE (Unknown file type)\n");
            break;
        case ET_REL:
            printf("REL (Relocatable file)\n");
            break;
        case ET_EXEC:
            printf("EXEC (Executable file)\n");
            break;
        case ET_DYN:
            printf("DYN (Shared object file)\n");
            break;
        case ET_CORE:
            printf("CORE (Core file)\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }

    printf("  Machine:                           ");
    switch (ehdr32.e_machine) {
        case EM_386:
            printf("Intel 80386\n");
            break;
        case EM_SPARC:
            printf("Sparc\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }

    printf("  Version:                           0x%x\n", ehdr32.e_version);
    printf("  Entry point address:               0x%lx\n", (unsigned long)ehdr32.e_entry);
    printf("  Start of program headers:          %lu (bytes into file)\n", (unsigned long)ehdr32.e_phoff);
    printf("  Start of section headers:          %lu (bytes into file)\n", (unsigned long)ehdr32.e_shoff);
    printf("  Flags:                             0x%x\n", ehdr32.e_flags);
    printf("  Size of this header:               %u (bytes)\n", ehdr32.e_ehsize);
    printf("  Size of program headers:           %u (bytes)\n", ehdr32.e_phentsize);
    printf("  Number of program headers:         %u\n", ehdr32.e_phnum);
    printf("  Size of section headers:           %u (bytes)\n", ehdr32.e_shentsize);
    printf("  Number of section headers:         %u\n", ehdr32.e_shnum);
    printf("  Section header string table index: %u\n", ehdr32.e_shstrndx);
}
