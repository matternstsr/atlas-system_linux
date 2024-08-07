#ifndef HREADELF_H
#define HREADELF_H

#include <limits.h>
#include <fcntl.h>
#include <elf.h>         /* ELF file format structures and constants */
#include <errno.h>       /* Error number definitions */
#include <stdbool.h>     /* Boolean type and values */
#include <stdint.h>      /* Integer types */
#include <string.h>      /* String manipulation functions */
#include <stdio.h>       /* std func (fseek, fread, printf, sprintf, putchar)*/
#include <stdlib.h>      /* General utilities (memory management functions) */
#include <sys/types.h>   /* Data types */
#include <sys/stat.h>    /* File status (stat, S_ISREG macro) */
#include <unistd.h>      /* POSIX API (system calls like file manipulation) */

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define USAGE "hnm elf_filename\n"
#define ERR_PREFIX "%s: "
#define ERR_NO_ENTRY ERR_PREFIX "'%s': No such file\n"
#define ERR_NO_ACCESS ERR_PREFIX "%s: Permission denied\n"
#define ERR_NOT_MAGIC ERR_PREFIX "File format not recognized\n"
#define IS_32(x) ((x).e_ident[EI_CLASS] == ELFCLASS32)
#define IS_64 ((elf_header->e64).e_ident[EI_CLASS] == ELFCLASS64)
#define Is_BigE(x) ((x).e_ident[EI_DATA] == ELFDATA2MSB)
#define EGET(x) (IS_32(elf_header->e64) ? elf_header->e32.x : \
					elf_header->e64.x)
#define SGET(i, x) (IS_32(elf_header->e64) ? elf_header->s32[i].x : \
					elf_header->s64[i].x)
#define PGET(i, x) (IS_32(elf_header->e64) ? elf_header->p32[i].x : \
					elf_header->p64[i].x)
#define YGET(i, x) (IS_32(elf_header->e64) ? elf_header->y32[i].x : \
					elf_header->y64[i].x)
#define BFD_NO_FLAGS 0x00
#define HAS_LINENO 0x04
#define HAS_DEBUG 0x08
#define HAS_LOCALS 0x20
#define WP_TEXT 0x80
#define HAS_RELOC 0x01 /* Object file has relocation entries */
#define EXEC_P 0x02  /* Executable file */
#define HAS_SYMS 0x10 /* Object file has symbol table entries */
#define DYNAMIC 0x40  /* Shared object file */
#define D_PAGED 0x100 /* Segment data is paged */

/* Define multi-line string for format legend */
#define FORMAT_LEGEND \
	"Key to Flags:\n" \
	"  W (write), A (alloc), X (execute), M (merge), S (strings)%s\n" \
	"  I (info), L (link order), G (group), T (TLS)," \
	"  E (exclude), x (unknown)\n" \
	"  O (extra OS processing required) o (OS specific)," \
	"  p (processor specific)\n"
/* Define title format for 32-bit sections */
#define TITLE_SECTION_32 \
	"  [Nr] Name              Type            Addr     Off    Size   " \
	"ES Flg Lk Inf Al\n"
/* Define format for 32-bit sections */
#define FORMAT_SECTION_32 \
	"  [%2u] %-17s %-15.15s %8.8lx %6.6lx %6.6lx %2.2lx %3s %2u %3u %2lu\n"
/* Define title format for 64-bit sections */
#define TITLE_SECTION_64 \
	"  [Nr] Name              Type            Address          Off    Size   " \
	"ES Flg Lk Inf Al\n"
/* Define format for 64-bit sections */
#define FORMAT_SECTION_64 \
	"  [%2u] %-17s %-15.15s %16.16lx %6.6lx %6.6lx %2.2lx %3s %2u %3u %2lu\n"
/* Define format for 64-bit symbols */
#define FORMAT_SYMBOL_64 \
	"%16.16lx %c %s"
/* Define format for 32-bit symbols */
#define FORMAT_SYMBOL_32 \
	"%8.8lx %c %s"

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

/** Structure to hold the state of the objdump tool */
typedef struct objdump_state_s
{
	char *exec_name;    /* Name of the executable */
	char *f_name;       /* Name of the file being analyzed */
	FILE *f_stream;     /* File stream pointer */
	int f_size;         /* Size of the file */
	bool big_endian;    /* Indicates big endian format */
	bool ELF_32bit;     /* Indicates ELF 32-bit format */
	Elf64_Ehdr f_header;/* ELF header structure */
	Elf64_Shdr *s_headers; /* Section headers */
	char *sh_strtab;    /* String table for section headers */
} objdump_state;

int empty_sections(elf_t *elf_header, int fd, size_t *num_printed);
size_t empty_section(elf_t *elf_header, int fd, size_t i, char *string_table);
char *check_format(elf_t *elf_header);
void essential_information(elf_t *elf_header, char *string_table);
int name_search(elf_t *elf_header, char *string_table, char *section_name);
void ef_flags(unsigned long flags);
int crack_open_file(char *name, int silent, char **argv);
int is_elf_file(char *elf_header);
void print_hex_memory(void *ptr, size_t n);
unsigned short swap_endian_uint16(unsigned short n);
unsigned int swap_endian_uint32(unsigned int n);
unsigned long swap_endian_uint64(unsigned long n);
void swap_all_endian(elf_t *h);
void swap_all_endian_section(elf_t *h, size_t i);
void swap_all_endian_program(elf_t *h, size_t i);
void swap_all_endian_symbol(elf_t *h, size_t i);
void swap_all_endian_verneed(elf_t *h, uint16_t *versym, size_t versym_size,
	Elf64_Verneed *verneed, size_t verneed_size);
int print_header(elf_t *elf_header);
int print_magic(Elf64_Ehdr *elf_header);
int print_class(Elf64_Ehdr *elf_header);
int print_data(Elf64_Ehdr *elf_header);
int print_version(Elf64_Ehdr *elf_header);
int print_osabi(Elf64_Ehdr *elf_header);
int print_osabi_more(Elf64_Ehdr *elf_header);
int print_abiversion(Elf64_Ehdr *elf_header);
int print_type(elf_t *elf_header);
int print_entry(elf_t *elf_header);
int print_machine(Elf64_Ehdr *elf_header);
int print_e_version(elf_t *elf_header);
int print_program_headers(elf_t *elf_header);
char *get_machine(Elf64_Ehdr *elf_header);
char *get_machine2(Elf64_Ehdr *elf_header);
int print_section_headers(elf_t *elf_header);
int print_flags(elf_t *elf_header);
int print_section_headers_full(elf_t *elf_header, int fd);
void read_section_headers(elf_t *elf_header, int fd);
char *read_string_table(elf_t *elf_header, int fd);
char *get_section_type(unsigned int sh_type);
void print_section_headers32(elf_t *elf_header, char *string_table);
void print_section_headers64(elf_t *elf_header, char *string_table);
char *get_section_flags(elf_t *elf_header, size_t i);
const char *get_elf_file_type(elf_t *elf_header);
int print_program_headers_full(elf_t *elf_header, int fd);
void print_program_headers32(elf_t *elf_header, char *string_table, int fd);
void print_program_headers64(elf_t *elf_header, char *string_table, int fd);
void read_program_headers(elf_t *elf_header, int fd);
char *get_segment_type(unsigned long p_type);
int print_section_to_segment_mapping(elf_t *elf_header, char *string_table);
int print_all_symbol_tables(elf_t *elf_header, int fd, size_t *num_printed);
size_t print_symbol_table(elf_t *elf_header, int fd, size_t i,
							char *string_table);
size_t print_symbol_table32(elf_t *elf_header, char *string_table,
							char *sym_string_table, uint16_t *versym,
							Elf64_Verneed *verneed, size_t verneed_size,
							int section);
size_t print_symbol_table64(elf_t *elf_header, char *string_table,
							char *sym_string_table, uint16_t *versym,
							Elf64_Verneed *verneed, size_t verneed_size,
							int section);
void print_verneed_info(elf_t *elf_header, char *sym_string_table,
						uint16_t *versym, Elf64_Verneed *verneed,
						size_t verneed_size, size_t i, size_t size,
						int section);
void read_symbol_table(elf_t *elf_header, int fd, int i);
char *read_symbol_string_table(elf_t *elf_header, int fd, int i);
size_t find_verneed_index(Elf64_Verneed *verneed, size_t verneed_size,
							size_t index);
void print_verneed_table(elf_t *elf_header, int fd, int i,
							char *sym_string_table, uint16_t *versym,
							Elf64_Verneed *verneed);
void *read_data(elf_t *elf_header, int fd, unsigned long offset, long size);
char *get_sym_type(elf_t *elf_header, size_t i);
char *get_sym_bind(elf_t *elf_header, size_t i);
char *get_sym_visibility(elf_t *elf_header, size_t i);
char get_nm_type32(Elf32_Sym sym, Elf32_Shdr *shdr);
char get_nm_type64(Elf64_Sym sym, Elf64_Shdr *shdr);
int process_file(char *file_name, int multiple, char **argv);
int open_and_validate_elf(char *file_name, elf_t *elf_header, char **argv);
int process_and_print_symbols(elf_t *elf_header, int fd, char **argv,
							char *file_name);

/** Open and initialize ELF file */
int openELF(objdump_state *state);
/** Print error message */
void errorMsg(char *format, char *err_str, objdump_state *state);
/** Initialize objdump_state structure */
void initState(objdump_state *state);
/** Clean up and close ELF file */
void closeState(objdump_state *state);
/** Read ELF file header */
int readElfFileHeader(objdump_state *state);
/** Read 32-bit ELF header */
int readElf32Header(objdump_state *state);
/** Read 64-bit ELF header */
int readElf64Header(objdump_state *state);
/** Swap endianness of ELF64 header */
void swapEndianElf64Header(Elf64_Ehdr *ehdr64);
/** Swap endianness of ELF32 header */
void swapEndianElf32Header(Elf32_Ehdr *ehdr32);
/** Read section headers */
int getSecHeaders(objdump_state *state);
/** Read 64-bit section headers */
int get64bitSecHeaders(objdump_state *state);
/** Read 32-bit section headers */
int get32bitSecHeaders(objdump_state *state);
/** Byte-swap ELF64 section header */
void bswapElf64_Shdr(Elf64_Shdr *shdr64);
/** Byte-swap ELF32 section header */
void bswapElf32_Shdr(Elf32_Shdr *shdr32);
/** Get section header string table */
int getSecHeadStrTab(objdump_state *state);
/** Get machine string of the file */
char *fileMachString(objdump_state *state);
/** Get architecture string of the file */
char *fileArchString(objdump_state *state);
/** Set and return file flags */
uint32_t setFileFlags(objdump_state *state);
/** Print file flags */
void printFileFlags(uint32_t flags);
/** Print information about the file */
int printFileInfo(objdump_state *state);
/** Allocate memory for section contents */
unsigned char *getSecBuff(objdump_state *state, Elf64_Shdr *section);
/** Print formatted section contents */
void printSecBuff(unsigned char *buff, Elf64_Shdr *section);
/** Print a row of section contents */
void printSecBuffRow(unsigned char *buff, uint32_t row, uint32_t bytes);
/** Print contents of all sections */
int printSections(objdump_state *state);

#endif
