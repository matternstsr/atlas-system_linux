#ifndef ELF_H
#define ELF_H

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

#ifndef EM_SPARCBIGENDIAN32
#define EM_SPARCBIGENDIAN32 0x6001
#endif

/* Function prototypes */
void readelf_header32(const char *filename, bool flip_endian);
void readelf_header64(const char *filename, bool flip_endian);
void print_machine_type(uint16_t machine, bool flip_endian);
void print_osabi(unsigned char osabi);
void print_file_type(uint16_t type);
void print_machine_type(uint16_t machine, bool flip_endian);

#endif /* ELF_H */
