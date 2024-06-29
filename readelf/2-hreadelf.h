#ifndef H_1_HREADELF_H
#define H_1_HREADELF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <elf.h>

// Function prototypes
void process_elf_file(const char *filename);
int parse_elf_header(FILE *file);
void print_section_headers(Elf64_Ehdr *elf_header,
                            Elf64_Shdr *section_headers, int num_sections);
// Add more function prototypes as needed

// Constants
#define MAX_SECTIONS 100
#define ELFCLASS32 1
#define ELFCLASS64 2

// Structures
typedef struct {
    uint32_t offset;
    uint32_t size;
    uint32_t type;
} SectionHeader;

// Global variables
extern int verbose_mode;

#endif // H_1_HREADELF_H
