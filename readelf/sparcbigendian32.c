/* sparcbigendian32.c */

#include "myelf.h"

/* Function to print ELF header information */
void sparcbigendian32(const char *filename) {
    int fd, i;
    MyElf32_Ehdr ehdr;

    fd = open_elf(filename); /* Open ELF file */
    read_elf_header(fd, &ehdr); /* Read ELF header */

    /* Print ELF Header */
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (i = 0; i < EI_NIDENT; ++i) {
        printf("%02x ", ehdr.e_ident[i]); /* Print magic bytes */
    }
    printf("\n");

    printf("  Class:                             ELF32\n");
    printf("  Data:                              2's complement, big endian\n");
    printf("  Version:                           %u (current)\n", (unsigned int)ehdr.e_ident[EI_VERSION]);

    print_osabi(ehdr.e_ident[EI_OSABI]); /* Print OS/ABI */
    printf("  ABI Version:                       %u\n", (unsigned int)ehdr.e_ident[EI_ABIVERSION]);
    print_file_type(ehdr.e_type); /* Print file type */
    print_machine_type(ehdr.e_machine); /* Print machine type */

    printf("  Version:                           0x%x\n", bswap_32(ehdr.e_version)); /* Print version */
    printf("  Entry point address:               0x%lx\n", (unsigned long)bswap_32(ehdr.e_entry)); /* Print entry point address */
    printf("  Start of program headers:          %u (bytes into file)\n", bswap_32(ehdr.e_phoff)); /* Print program headers start */
    printf("  Start of section headers:          %u (bytes into file)\n", bswap_32(ehdr.e_shoff)); /* Print section headers start */
    printf("  Flags:                             0x%x\n", bswap_32(ehdr.e_flags)); /* Print flags */
    printf("  Size of this header:               %u (bytes)\n", bswap_16(ehdr.e_ehsize)); /* Print size of ELF header */
    printf("  Size of program headers:           %u (bytes)\n", bswap_16(ehdr.e_phentsize)); /* Print size of program headers */
    printf("  Number of program headers:         %u\n", bswap_16(ehdr.e_phnum)); /* Print number of program headers */
    printf("  Size of section headers:           %u (bytes)\n", bswap_16(ehdr.e_shentsize)); /* Print size of section headers */
    printf("  Number of section headers:         %u\n", bswap_16(ehdr.e_shnum)); /* Print number of section headers */
    printf("  Section header string table index: %u\n", bswap_16(ehdr.e_shstrndx)); /* Print section header string table index */
}
