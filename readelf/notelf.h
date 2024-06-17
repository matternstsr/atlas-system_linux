#ifndef ELF_H
#define ELF_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>


/* ELF header structures for both 32-bit and 64-bit formats */
/* Function prototypes */
void readelf_header(const char *filename);

#endif /* ELF_H */
