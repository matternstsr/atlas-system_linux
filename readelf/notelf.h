#ifndef ELF_H
#define ELF_H

#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>


/* atlas-system_linux/readelf/notelf.h */

/* ELF header structures for both 32-bit and 64-bit formats */
/* Function prototypes */
void readelf_header(const char *filename);
void machine_32_printing(Elf32_Ehdr e_hdr);
void type_32_printing(Elf32_Ehdr e_hdr);
/* int is_little_endian();
int is_big_endian(); */

#endif /* ELF_H */
