#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>


void readelf_header(const char *filename) {
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

    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (i = 0; i < EI_NIDENT; ++i) {
        printf("%02x ", ehdr.e_ident[i]);
    }
    printf("\n");

    printf("  Class:                             %s\n", (ehdr.e_ident[EI_CLASS] == ELFCLASS64) ? "ELF64" : "ELF32");
    printf("  Data:                              %s\n", (ehdr.e_ident[EI_DATA] == ELFDATA2LSB) ? "2's complement, little endian" : "2's complement, big endian");
    printf("  Version:                           %u (current)\n", (unsigned int)ehdr.e_ident[EI_VERSION]);

    switch (ehdr.e_ident[EI_OSABI]) {
        case ELFOSABI_SYSV:
            printf("  OS/ABI:                            UNIX - System V\n");
            break;
        case ELFOSABI_NETBSD:
            printf("  OS/ABI:                            UNIX - NetBSD\n");
            break;
        case ELFOSABI_SOLARIS:
            printf("  OS/ABI:                            UNIX - Solaris\n");
            break;
        default:
            printf("  OS/ABI:                            <unknown: %x>\n", (unsigned int)ehdr.e_ident[EI_OSABI]);
            break;
    }

    printf("  ABI Version:                       %u\n", (unsigned int)ehdr.e_ident[EI_ABIVERSION]);

    printf("  Type:                              ");
    switch (ehdr.e_type) {
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
    switch (ehdr.e_machine) {
        case EM_386:
            printf("Intel 80386\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }

    printf("  Version:                           0x%x\n", ehdr.e_version);
    printf("  Entry point address:               0x%x\n", ehdr.e_entry);
    printf("  Start of program headers:          %u (bytes into file)\n", ehdr.e_phoff);
    printf("  Start of section headers:          %u (bytes into file)\n", ehdr.e_shoff);
    printf("  Flags:                             0x%x\n", ehdr.e_flags);
    printf("  Size of this header:               %u (bytes)\n", ehdr.e_ehsize);
    printf("  Size of program headers:           %u (bytes)\n", ehdr.e_phentsize * ehdr.e_phnum);
    printf("  Number of program headers:         %u\n", ehdr.e_phnum);
    printf("  Size of section headers:           %u (bytes)\n", ehdr.e_shentsize * ehdr.e_shnum);
    printf("  Number of section headers:         %u\n", ehdr.e_shnum);
    printf("  Section header string table index: %u\n", ehdr.e_shstrndx);
}
