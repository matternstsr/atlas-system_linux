#ifndef HREADELF_H
#define HREADELF_H

#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define USAGE "0-hreadelf elf_filename\n"
#define MYNAME "0-hreadelf"

#define ERR_PREFIX MYNAME ": Error: "
#define ERR_NO_ENTRY ERR_PREFIX \
	"'%s': No such file\n"
#define ERR_NO_ACCESS ERR_PREFIX \
	"Input file '%s' is not readable.\n"
#define ERR_NOT_MAGIC ERR_PREFIX \
	"Not an ELF file - it has the wrong magic bytes at the start\n"

#define PROGRAM_TITLE \
	"\nElf file type is %s\nEntry point 0x%lx\nThere are %d program headers," \
	" starting at offset %lu\n"
/* #define IS_64 ((EH->e64).e_ident[EI_CLASS] == ELFCLASS64) */
/* #define IS_64 ((EH->e64).e_ident [EI_CLASS] == ELFCLASS64) */
#define IS_64 ((EH->e64).e_ident[EI_CLASS] == ELFCLASS64)
#define IS_32(x) ((x).e_ident[EI_CLASS] == ELFCLASS32)
#define IS_BE(x) ((x).e_ident[EI_DATA] == ELFDATA2MSB)
#define EGET(x) \
	(IS_32(EH->e64) ? EH->e32.x : EH->e64.x)
#define SGET(i, x) \
	(IS_32(EH->e64) ? EH->s32[i].x : EH->s64[i].x)
#define PGET(i, x) \
	(IS_32(EH->e64) ? EH->p32[i].x : EH->p64[i].x)
#define YGET(i, x) \
	(IS_32(EH->e64) ? EH->y32[i].x : EH->y64[i].x)
#define FORMAT_LEGEND \
	"Key to Flags:\n" \
	"  W (write), A (alloc), X (execute), M (merge), S (strings)%s\n" \
	"  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n" \
	"  O (extra OS processing required) o (OS specific), p (processor specific)\n"
#define TITLE_SECTION_32 \
	"  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n"
#define FORMAT_SECTION_32 \
	"  [%2u] %-17s %-15.15s %8.8lx %6.6lx %6.6lx %2.2lx %3s %2u %3u %2lu\n"
#define TITLE_SECTION_64 \
	"  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n"
#define FORMAT_SECTION_64 \
	"  [%2u] %-17s %-15.15s %16.16lx %6.6lx %6.6lx %2.2lx %3s %2u %3u %2lu\n"
#define PROGRAM_TITLE \
	"\nElf file type is %s\nEntry point 0x%lx\nThere are %d program headers," \
	" starting at offset %lu\n"

#define TITLE_PROGRAM_32 \
	"  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align\n"

#define FORMAT_PROGRAM_32 \
	"  %-14s 0x%6.6lx 0x%8.8lx 0x%8.8lx 0x%5.5lx 0x%5.5lx %c%c%c %#lx\n"

#define TITLE_PROGRAM_64 \
	"  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align\n"

#define FORMAT_PROGRAM_64 \
	"  %-14s 0x%6.6lx 0x%16.16lx 0x%16.16lx 0x%6.6lx 0x%6.6lx %c%c%c %#lx\n"
#define MAPPING_TITLE \
	"\n Section to Segment mapping:\n  Segment Sections...\n"

#define ELF_SECTION_SIZE(sec_hdr, segment)			\
	(((sec_hdr->sh_flags & SHF_TLS) == 0				\
	|| sec_hdr->sh_type != SHT_NOBITS				\
	|| segment->p_type == PT_TLS) ? sec_hdr->sh_size : 0)

#define ELF_IS_SECTION_IN_SEGMENT(sec_hdr, segment)			\
	(((((sec_hdr->sh_flags & SHF_TLS) != 0)				\
	&& (segment->p_type == PT_TLS					\
	 || segment->p_type == PT_LOAD))				\
	|| ((sec_hdr->sh_flags & SHF_TLS) == 0				\
	&& segment->p_type != PT_TLS))					\
	&& (sec_hdr->sh_type == SHT_NOBITS					\
	|| (sec_hdr->sh_offset >= segment->p_offset		\
	   && (sec_hdr->sh_offset + ELF_SECTION_SIZE(sec_hdr, segment)	\
	       <= segment->p_offset + segment->p_filesz)))		\
	&& ((sec_hdr->sh_flags & SHF_ALLOC) == 0				\
	|| (sec_hdr->sh_addr >= segment->p_vaddr				\
	   && (sec_hdr->sh_addr + ELF_SECTION_SIZE(sec_hdr, segment)	\
	       <= segment->p_vaddr + segment->p_memsz))))

#define ELF_IS_SECTION_IN_SEGMENT_FILE(sec_hdr, segment)	\
	(sec_hdr->sh_size > 0						\
	&& ELF_IS_SECTION_IN_SEGMENT(sec_hdr, segment))

#define ELF_IS_SECTION_IN_SEGMENT_MEMORY(sec_hdr, segment)	\
	(ELF_SECTION_SIZE(sec_hdr, segment) > 0			\
	&& ELF_IS_SECTION_IN_SEGMENT(sec_hdr, segment))

#define MY_UNKNOWN_SECTION_TYPE 0xFF00


/**
 * struct Elf - stores 32/64 structs and other data
 * @e64: the 64 bit struct
 * @e32: the 32 bit struct
 * @s64: the 64 bit struct section array
 * @s32: the 32 bit struct section array
 * @p64: the 64 bit struct program array
 * @p32: the 32 bit struct program array
 * @y64: the 64 bit struct symbol array
 * @y32: the 32 bit struct symbol array
 */
typedef struct Elf
{
	Elf64_Ehdr e64;
	Elf32_Ehdr e32;
	Elf64_Shdr *s64;
	Elf32_Shdr *s32;
	Elf64_Phdr *p64;
	Elf32_Phdr *p32;
	Elf64_Sym *y64;
	Elf32_Sym *y32;

} elf_t;

char *get_unknown_section_type_name(unsigned int sh_type);
/* static char *get_unknown_section_type_name(unsigned int sh_type); */
int read_and_validate_elf_header(int fd, elf_t *EH);
/* int display_all_elf_program_headers(const elf_t *EH, int fd); */
int open_and_handle_file(char *name, int silent);
int check_elf_header(char *EH);
void print_bytes(void *ptr, size_t n);
unsigned short switch_2_byte(unsigned short n);
unsigned int seu32(unsigned int n);
unsigned long seu64(unsigned long n);
void switch_all_endian(elf_t *EH);
void switch_endianness_sections_32(elf_t *EH, size_t index);
void switch_endianness_sections_64(elf_t *EH, size_t index);
void switch_endianness_sections(elf_t *EH, size_t index);
void switch_endianness_program_headers_32(elf_t *EH, size_t index);
void switch_endianness_program_headers_64(elf_t *EH, size_t index);
void switch_endianness_program_headers(elf_t *EH, size_t index);
void switch_all_endian_symbol(elf_t *h, size_t i);
void switch_all_endian_ver(elf_t *h, uint16_t *versym, size_t versym_size,
	Elf64_Verneed *verneed, size_t verneed_size);
int print_header(elf_t *EH);
int print_magic(Elf64_Ehdr *EH);
int print_class(Elf64_Ehdr *EH);
int print_data(Elf64_Ehdr *EH);
int print_version(Elf64_Ehdr *EH);
int print_osabi(Elf64_Ehdr *EH);
int print_osabi_more(Elf64_Ehdr *EH);
int print_abiversion(Elf64_Ehdr *EH);
int print_type(elf_t *EH);
int print_entry(elf_t *EH);
int print_machine(Elf64_Ehdr *EH);
int print_e_version(elf_t *EH);
int print_program_headers(elf_t *EH);
char *get_machine(Elf64_Ehdr *EH);
char *get_machine2(Elf64_Ehdr *EH);
int print_section_headers(elf_t *EH);
int print_flags(elf_t *EH);
int print_section_headers_full(elf_t *EH, int fd);
void read_elf_section_headers(elf_t *EH, int fd);
char *read_elf_string_table(elf_t *EH, int fd);
char *get_section_type_name(unsigned int sh_type);
void print_elf_section_headers_32(elf_t *EH, char *string_table);
void print_elf_section_headers_64(elf_t *EH, char *string_table);
char *translate_section_flags(elf_t *EH, size_t i);
char *get_elf_file_type(elf_t *EH);
int display_all_elf_program_headers(elf_t *EH, int fd);
void print_program_headers_32bit(elf_t *EH,
								char *string_table, int fd);
void print_program_headers_64bit(elf_t *EH,
								char *string_table, int fd);
void read_program_headers_from_file(elf_t *EH, int fd);
char *get_segment_type(unsigned long p_type);
int print_section_segment_mappings(elf_t *EH, char *string_table);
int print_all_symbol_tables(elf_t *EH, int fd);
void print_symbol_table(elf_t *EH, int fd, size_t i,
	char *string_table);
void print_symbol_table32(elf_t *EH, char *string_table,
	char *sym_string_table, uint16_t *versym, Elf64_Verneed *verneed,
	size_t verneed_size, int section);
void print_symbol_table64(elf_t *EH, char *string_table,
	char *sym_string_table, uint16_t *versym, Elf64_Verneed *verneed,
	size_t verneed_size, int section);
void print_verneed_info(elf_t *EH, char *sym_string_table,
	uint16_t *versym, Elf64_Verneed *verneed, size_t verneed_size, size_t i,
	size_t size, int section);

void read_symbol_table(elf_t *EH, int fd, int i);
char *read_symbol_string_table(elf_t *EH, int fd, int i);
size_t find_verneed_index(Elf64_Verneed *verneed, size_t verneed_size,
	size_t index);
void print_verneed_table(elf_t *EH, int fd, int i,
	char *sym_string_table, uint16_t *versym, Elf64_Verneed *verneed);
void *read_data(elf_t *EH, int fd, unsigned long offset, long size);
char *get_sym_type(elf_t *EH, size_t i);
char *get_sym_bind(elf_t *EH, size_t i);
char *get_sym_visibility(elf_t *EH, size_t i);
int check_elf(char *EH);
void switch_all_endian(elf_t *EH);
int print_program_headers_full(elf_t *EH, int fd);
int open_file(char *name, int silent);

#endif
