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
#include <string.h>
#include <byteswap.h>
#include <sys/mman.h>
#include <stddef.h>

/* Define constants not available in <elf.h> */

#define ELFOSABI_SORTIX 9
#define EM_SPARC32PLUS 18
#define EM_SPARCV9 43
#define MAX_SECTIONS 1000

/* Function prototypes */
void readelf_header32(const char *filename);
void readelf_header64(const char *filename);
void print_osabi(unsigned char osabi);
void print_file_type(uint16_t type);
void print_machine_type(uint16_t machine);
void sparcbigendian32(const char *filename);

/* Function prototypes */
void readelf_header32(const char *filename);
void readelf_header64(const char *filename);

/* Utility functions for endian swapping */
uint16_t swap16(uint16_t val, bool swap);
uint32_t swap32(uint32_t val, bool swap);

/* Function to read and display section headers */
void readelf_sections(FILE *fp);

/* Function to map section type to string name */
extern const char *section_type_to_string(unsigned int type);

/* Function to convert flags to string representation */
extern const char *flags_to_string(unsigned long flags);

#endif /* ELF_H */
