#ifndef __1READELF_H__
#define __1READELF_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>
#include <byteswap.h>

#define SHT_VERDEF 0x6ffffffd

typedef struct
{
	uint32_t type;
	const char *name;
} SectionTypeMapping;

typedef struct
{
	uint32_t type;
	const char *name;
} SpecialSectionTypeMapping;

Elf64_Shdr *read_section_headers(FILE *fp, Elf64_Ehdr *ehdr);
void display_section_headers(Elf64_Ehdr *ehdr,
	Elf64_Shdr *shdr, char *shstrtab);
void print_32bit_sec_hdrs(Elf32_Ehdr *ehdr,
                          Elf32_Shdr *shdr,
                          const char *strtab);
void print_64bit_sec_headers(Elf64_Ehdr *ehdr,
                             Elf64_Shdr *shdr,
                             const char *strtab);
void *map_file_into_memory(const char *filename, size_t *filesize);
void check_elf_magic(Elf64_Ehdr *ehdr);
void analyze_prnt_elf_hdrs(void *maps, size_t filesize);
void proc_prnt_elf64_sec(Elf64_Ehdr *ehdr, int is_big_endian, void *maps);
void proc_prnt_elf32_sec(Elf32_Ehdr *ehdr32, int is_big_endian, void *maps);
void swap_endianess_64(Elf64_Shdr *shdr, int shnum);
void swap_endianess_32(Elf32_Shdr *shdr, int shnum);
void print_flag_key(void);
const char *convert_flags_to_string(uint64_t flags);
const char *resolve_section_type(uint32_t sh_type);
const char *res_spec_sec_type_name(uint32_t sh_type);
const char *resolve_std_sec_type_name(uint32_t sh_type);
const char *map_spec_sec_type_name(uint32_t sh_type);
void print_elf_flags_key(void);
int read_sparcbigendian32(const char *filename, Elf32_Ehdr *ehdr);
void print_elf_header(const Elf32_Ehdr *ehdr);
void sparcbigendian32(const char *filename);

#endif
