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

/* Utility functions for endian swapping */
uint16_t swap16(uint16_t val, bool swap)
{
	return (swap ? ((val << 8) | (val >> 8)) : val);
}

uint32_t swap32(uint32_t val, bool swap)
{
	if (swap)
	{
		return (((val << 24) & 0xFF000000) |
			((val <<  8) & 0x00FF0000) |
			((val >>  8) & 0x0000FF00) |
			((val >> 24) & 0x000000FF));
	}
	return (val);
}

/* Function prototypes */
void readelf_header32(const char *filename);
void readelf_header64(const char *filename);
void sparcbigendian32(const char *filename);

#endif /* ELF_H */
