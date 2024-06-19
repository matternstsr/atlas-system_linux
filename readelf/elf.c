#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>

// Define missing constants if not available in <elf.h>
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
    Elf64_Ehdr ehdr64;
    
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (read(fd, &ehdr64, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr)) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (i = 0; i < EI_NIDENT; ++i) {
        printf("%02x ", ehdr64.e_ident[i]);
    }
    printf("\n");

    printf("  Class:                             %s\n", (ehdr64.e_ident[EI_CLASS] == ELFCLASS64) ? "ELF64" : "ELF32");
    printf("  Data:                              %s\n", (ehdr64.e_ident[EI_DATA] == ELFDATA2LSB) ? "2's complement, little endian" : "2's complement, big endian");
    printf("  Version:                           %u (current)\n", (unsigned int)ehdr64.e_ident[EI_VERSION]);

    switch (ehdr64.e_ident[EI_OSABI]) {
        case ELFOSABI_SYSV:
            printf("  OS/ABI:                            UNIX - System V\n");
            break;
        case ELFOSABI_NETBSD:
            printf("  OS/ABI:                            UNIX - NetBSD\n");
            break;
        case ELFOSABI_SOLARIS:
            printf("  OS/ABI:                            UNIX - Solaris\n");
            break;
        case ELFOSABI_SORTIX:
            printf("  OS/ABI:                            UNIX - Sortix\n");
            break;
        default:
            printf("  OS/ABI:                            <unknown: %x>\n", (unsigned int)ehdr64.e_ident[EI_OSABI]);
            break;
    }

    printf("  ABI Version:                       %u\n", (unsigned int)ehdr64.e_ident[EI_ABIVERSION]);

    printf("  Type:                              ");
    switch (ehdr64.e_type) {
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
    switch (ehdr64.e_machine) {
        case EM_X86_64:
            printf("Advanced Micro Devices X86-64\n");
            break;
        case EM_386:
            printf("Intel 80386\n");
            break;
        case EM_SPARC:
            printf("Sparc\n");
            break;
        case EM_SPARC32PLUS:
            printf("Sparc v8+\n");
            break;
        case EM_SPARCV9:
            printf("Sparc v9\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }

    /* Additional handling for specific OS/ABI values */
    if (ehdr64.e_ident[EI_OSABI] == ELFOSABI_SORTIX && ehdr64.e_ident[EI_CLASS] == ELFCLASS32) {
        printf("  Version:                           0x%x\n", ehdr64.e_version);
        printf("  Entry point address:               0x%lx\n", (unsigned long)ehdr64.e_entry);
        printf("  Start of program headers:          %lu (bytes into file)\n", (unsigned long)ehdr64.e_phoff);
        printf("  Start of section headers:          %lu (bytes into file)\n", (unsigned long)ehdr64.e_shoff);
        printf("  Flags:                             0x%x\n", ehdr64.e_flags);
        printf("  Size of this header:               %u (bytes)\n", ehdr64.e_ehsize);
        printf("  Size of program headers:           %u (bytes)\n", ehdr64.e_phentsize);
        printf("  Number of program headers:         %u\n", ehdr64.e_phnum);
        printf("  Size of section headers:           %u (bytes)\n", ehdr64.e_shentsize);
        printf("  Number of section headers:         %u\n", ehdr64.e_shnum);
        printf("  Section header string table index: %u\n", ehdr64.e_shstrndx);
    } else if (ehdr64.e_ident[EI_OSABI] == ELFOSABI_SOLARIS && ehdr64.e_ident[EI_CLASS] == ELFCLASS32) {
        printf("  Version:                           0x%x\n", ehdr64.e_version);
        printf("  Entry point address:               0x%lx\n", (unsigned long)ehdr64.e_entry);
        printf("  Start of program headers:          %lu (bytes into file)\n", (unsigned long)ehdr64.e_phoff);
        printf("  Start of section headers:          %lu (bytes into file)\n", (unsigned long)ehdr64.e_shoff);
        printf("  Flags:                             0x%x\n", ehdr64.e_flags);
        printf("  Size of this header:               %u (bytes)\n", ehdr64.e_ehsize);
        printf("  Size of program headers:           %u (bytes)\n", ehdr64.e_phentsize);
        printf("  Number of program headers:         %u\n", ehdr64.e_phnum);
        printf("  Size of section headers:           %u (bytes)\n", ehdr64.e_shentsize);
        printf("  Number of section headers:         %u\n", ehdr64.e_shnum);
        printf("  Section header string table index: %u\n", ehdr64.e_shstrndx);
    } else if (ehdr64.e_machine == EM_SPARC && ehdr64.e_ident[EI_CLASS] == ELFCLASS32) {
        printf("  Version:                           0x%x\n", ehdr64.e_version);
        printf("  Entry point address:               0x%lx\n", (unsigned long)ehdr64.e_entry);
        printf("  Start of program headers:          %lu (bytes into file)\n", (unsigned long)ehdr64.e_phoff);
        printf("  Start of section headers:          %lu (bytes into file)\n", (unsigned long)ehdr64.e_shoff);
        printf("  Flags:                             0x%x\n", ehdr64.e_flags);
        printf("  Size of this header:               %u (bytes)\n", ehdr64.e_ehsize);
        printf("  Size of program headers:           %u (bytes)\n", ehdr64.e_phentsize);
        printf("  Number of program headers:         %u\n", ehdr64.e_phnum);
        printf("  Size of section headers:           %u (bytes)\n", ehdr64.e_shentsize);
        printf("  Number of section headers:         %u\n", ehdr64.e_shnum);
        printf("  Section header string table index: %u\n", ehdr64.e_shstrndx);
    } else {
        printf("  Version:                           0x%x\n", ehdr64.e_version);
        printf("  Entry point address:               0x%lx\n", (unsigned long)ehdr64.e_entry);
        printf("  Start of program headers:          %lu (bytes into file)\n", (unsigned long)ehdr64.e_phoff);
        printf("  Start of section headers:          %lu (bytes into file)\n", (unsigned long)ehdr64.e_shoff);
        printf("  Flags:                             0x%x\n", ehdr64.e_flags);
        printf("  Size of this header:               %u (bytes)\n", ehdr64.e_ehsize);
        printf("  Size of program headers:           %u (bytes)\n", ehdr64.e_phentsize);
        printf("  Number of program headers:         %u\n", ehdr64.e_phnum);
        printf("  Size of section headers:           %u (bytes)\n", ehdr64.e_shentsize);
        printf("  Number of section headers:         %u\n", ehdr64.e_shnum);
        printf("  Section header string table index: %u\n", ehdr64.e_shstrndx);
    }
}
