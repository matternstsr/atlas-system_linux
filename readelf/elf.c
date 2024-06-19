/* atlas-system_linux/readelf/elf.c */

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

/* Function to print OS/ABI */
void print_osabi(unsigned char osabi) {
    printf("  OS/ABI:                            ");
    switch (osabi) {
        case ELFOSABI_SYSV:
            printf("UNIX - System V\n");
            break;
        case ELFOSABI_SOLARIS:
            printf("UNIX - Solaris\n");
            break;
        case ELFOSABI_SORTIX:
            printf("UNIX - Sortix\n");
            break;
        default:
            printf("<unknown: %x>\n", osabi);
            break;
    }
}

/* Function to print file type */
void print_file_type(uint16_t type) {
    printf("  Type:                              ");
    switch (type) {
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
}

/* Function to print machine type */
void print_machine_type(uint16_t machine) {
    printf("  Machine:                           ");
    switch (machine) {
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
}

/* Function to read and print ELF header for 64-bit ELF */
void readelf_header64(const char *filename) {
    int fd, i;
    Elf64_Ehdr ehdr;
    bool swap_endian = false;

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

    /* Determine endianness */
    if (ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
        swap_endian = true;

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
    print_file_type(swap16(ehdr.e_type, swap_endian));
    print_machine_type(swap16(ehdr.e_machine, swap_endian));

    printf("  Version:                           0x%x\n", ehdr.e_version);
    printf("  Entry point address:               0x%lx\n", (unsigned long)ehdr.e_entry);
    printf("  Start of program headers:          %lu (bytes into file)\n", (unsigned long)ehdr.e_phoff);
    printf("  Start of section headers:          %lu (bytes into file)\n", (unsigned long)ehdr.e_shoff);
    printf("  Flags:                             0x%x\n", ehdr.e_flags);
    printf("  Size of this header:               %u (bytes)\n", ehdr.e_ehsize);
    printf("  Size of program headers:           %u (bytes)\n", ehdr.e_phentsize);
    printf("  Number of program headers:         %u\n", ehdr.e_phnum);
    printf("  Size of section headers:           %u (bytes)\n", ehdr.e_shentsize);
    printf("  Number of section headers:         %u\n", ehdr.e_shnum);
    printf("  Section header string table index: %u\n", ehdr.e_shstrndx);
}

/* Function to read and print ELF header for 32-bit ELF */
void readelf_header32(const char *filename) {
    int fd, i;
    Elf32_Ehdr ehdr;
    bool swap_endian = false;

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

    /* Determine endianness */
    if (ehdr.e_ident[EI_DATA] == ELFDATA2MSB)
        swap_endian = true;

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
    print_file_type(swap16(ehdr.e_type, swap_endian));
    print_machine_type(swap16(ehdr.e_machine, swap_endian));

    printf("  Version:                           0x%x\n", ehdr.e_version);
    printf("  Entry point address:               0x%lx\n", (unsigned long)ehdr.e_entry);
    printf("  Start of program headers:          %lu (bytes into file)\n", (unsigned long)ehdr.e_phoff);
    printf("  Start of section headers:          %lu (bytes into file)\n", (unsigned long)ehdr.e_shoff);
    printf("  Flags:                             0x%x\n", ehdr.e_flags);
    printf("  Size of this header:               %u (bytes)\n", ehdr.e_ehsize);
    printf("  Size of program headers:           %u (bytes)\n", ehdr.e_phentsize);
    printf("  Number of program headers:         %u\n", ehdr.e_phnum);
    printf("  Size of section headers:           %u (bytes)\n", ehdr.e_shentsize);
    printf("  Number of section headers:         %u\n", ehdr.e_shnum);
    printf("  Section header string table index: %u\n", ehdr.e_shstrndx);
}
