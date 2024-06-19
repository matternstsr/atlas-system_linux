/* myelf.h */

#ifndef ELF_CUSTOM_H
#define ELF_CUSTOM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <byteswap.h>
#include <errno.h>

#define EI_NIDENT (16)

/* Define constants not available in <elf.h> */
#ifndef ELFOSABI_SORTIX
#define ELFOSABI_SORTIX 9
#endif

#ifndef EM_SPARC32PLUS
#define EM_SPARC32PLUS 18
#endif

#ifndef EM_SPARCV9
#define EM_SPARCV9 43
#endif

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} MyElf32_Ehdr;  /* Renamed struct to MyElf32_Ehdr to avoid conflict */

/* Function prototypes  */
int open_elf(const char *filename);
void read_elf_header(int fd, MyElf32_Ehdr *ehdr); /* Adjusted to use MyElf32_Ehdr */
void sparcbigendian32(const char *filename);
void print_osabi(unsigned char osabi);
void print_file_type(uint16_t e_type);
void print_machine_type(uint16_t e_machine);

#endif /* ELF_CUSTOM_H  */
