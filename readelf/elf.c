#include "notelf.h"
#include <stdint.h>
#include <stdbool.h>

/* Swap the endianness of a 16-bit integer */
uint16_t swap16(uint16_t val, bool swap) {
    return swap ? ((val << 8) | (val >> 8)) : val;
}

/* Swap the endianness of a 32-bit integer */
uint32_t swap32(uint32_t val, bool swap) {
    return swap ? (((val << 24) & 0xFF000000) |
                   ((val << 8) & 0x00FF0000) |
                   ((val >> 8) & 0x0000FF00) |
                   ((val >> 24) & 0x000000FF)) : val;
}

void machine_32_printing(Elf32_Ehdr e_hdr)
{
	unsigned int new;

	if (e_hdr.e_ident[EI_DATA] == ELFDATA2MSB)
		new = __bswap_16(e_hdr.e_machine);
	else
		new = e_hdr.e_machine;

	switch (new) {
        case EM_X86_64:
            printf("Advanced Micro Devices X86-64\n");
            break;
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
}

void readelf_header(const char *filename) {
    int fd;
    int i;
    Elf32_Ehdr ehdr32;  /* Assuming 32-bit ELF header for now */
    bool isUnixSystemV = false;
    
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
        isUnixSystemV = true;
        break;
    case ELFOSABI_SOLARIS:
        printf("UNIX - Solaris\n");
        break;
    default:
        printf("<unknown: %x>\n", (unsigned int)ehdr32.e_ident[EI_OSABI]);
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
    machine_32_printing(ehdr32);

    printf("  Version:                           0x%x\n", __bswap_32(ehdr32.e_version));
    printf("  Entry point address:               0x%x\n", __bswap_32(ehdr32.e_entry));
    printf("  Start of program headers:          %u (bytes into file)\n", swap32(ehdr32.e_phoff, isUnixSystemV));
    printf("  Start of section headers:          %u (bytes into file)\n", swap32(ehdr32.e_shoff, isUnixSystemV));
    printf("  Flags:                             0x%x\n", swap32(ehdr32.e_flags, isUnixSystemV));
    printf("  Size of this header:               %u (bytes)\n", swap16(ehdr32.e_ehsize, isUnixSystemV));
    printf("  Size of program headers:           %u (bytes)\n", swap16(ehdr32.e_phentsize, isUnixSystemV));
    printf("  Number of program headers:         %u\n", swap16(ehdr32.e_phnum, isUnixSystemV));
    printf("  Size of section headers:           %u (bytes)\n", swap16(ehdr32.e_shentsize, isUnixSystemV));
    printf("  Number of section headers:         %u\n", swap16(ehdr32.e_shnum, isUnixSystemV));
    printf("  Section header string table index: %u\n", swap16(ehdr32.e_shstrndx, isUnixSystemV));

/*
    printf("  Version:                           0x%x\n", (unsigned int)ehdr32.e_version);
    printf("  Entry point address:               0x%x\n", ehdr32.e_entry);
    printf("  Start of program headers:          %u (bytes into file)\n", swap32(ehdr32.e_phoff, isUnixSystemV));
    printf("  Start of section headers:          %u (bytes into file)\n", swap32(ehdr32.e_shoff, isUnixSystemV));
    printf("  Flags:                             0x%x\n", swap32(ehdr32.e_flags, isUnixSystemV));
    printf("  Size of this header:               %u (bytes)\n", swap16(ehdr32.e_ehsize, isUnixSystemV));
    printf("  Size of program headers:           %u (bytes)\n", swap16(ehdr32.e_phentsize, isUnixSystemV));
    printf("  Number of program headers:         %u\n", swap16(ehdr32.e_phnum, isUnixSystemV));
    printf("  Size of section headers:           %u (bytes)\n", swap16(ehdr32.e_shentsize, isUnixSystemV));
    printf("  Number of section headers:         %u\n", swap16(ehdr32.e_shnum, isUnixSystemV));
    printf("  Section header string table index: %u\n", swap16(ehdr32.e_shstrndx, isUnixSystemV)); */
}
