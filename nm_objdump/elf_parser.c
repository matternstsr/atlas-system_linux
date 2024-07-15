#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <elf.h>
#include "elf_parser.h"

#define MAX_SYMBOLS 1024

typedef struct {
    union {
        Elf32_Sym sym32;
        Elf64_Sym sym64;
    };
    char *name;
    int is_64bit;
} SymbolEntry;

static int compare_symbols(const void *a, const void *b) {
    const SymbolEntry *sa = (const SymbolEntry *)a;
    const SymbolEntry *sb = (const SymbolEntry *)b;
    return strcmp(sa->name, sb->name);
}

static const char *get_symbol_type_32(Elf32_Sym *sym) {
    switch (ELF32_ST_TYPE(sym->st_info)) {
        case STT_NOTYPE: return " ";
        case STT_OBJECT: return "B";
        case STT_FUNC: return "T";
        case STT_SECTION: return "S";
        case STT_FILE: return "f";
        case STT_COMMON: return "C";
        case STT_TLS: return "T";
        case STT_GNU_IFUNC: return "i";
        default: return "?";
    }
}

static const char *get_symbol_type_64(Elf64_Sym *sym) {
    switch (ELF64_ST_TYPE(sym->st_info)) {
        case STT_NOTYPE: return " ";
        case STT_OBJECT: return "B";
        case STT_FUNC: return "T";
        case STT_SECTION: return "S";
        case STT_FILE: return "f";
        case STT_COMMON: return "C";
        case STT_TLS: return "T";
        case STT_GNU_IFUNC: return "i";
        default: return "?";
    }
}

static int parse_symbols(FILE *file) {
    Elf64_Ehdr ehdr;
    if (fread(&ehdr, 1, sizeof(ehdr), file) != sizeof(ehdr)) {
        fprintf(stderr, "Error reading ELF header\n");
        return -1;
    }

    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not an ELF file\n");
        return -1;
    }

    int is_64bit = (ehdr.e_ident[EI_CLASS] == ELFCLASS64);

    if (!is_64bit) {
        fseek(file, 0, SEEK_SET);
        if (fread(&ehdr, 1, sizeof(Elf32_Ehdr), file) != sizeof(Elf32_Ehdr)) {
            fprintf(stderr, "Error reading 32-bit ELF header\n");
            return -1;
        }
    }

    fseek(file, is_64bit ? ehdr.e_shoff : ((Elf32_Ehdr*)&ehdr)->e_shoff, SEEK_SET);
    Elf64_Shdr shdr;
    Elf32_Shdr shdr32;
    if (fread(is_64bit ? (void*)&shdr : (void*)&shdr32, 1, is_64bit ? sizeof(shdr) : sizeof(shdr32), file) != (is_64bit ? sizeof(shdr) : sizeof(shdr32))) {
        fprintf(stderr, "Error reading section header\n");
        return -1;
    }

    Elf64_Shdr strtab_section_header;
    Elf32_Shdr strtab_section_header32;
    fseek(file, is_64bit ? ehdr.e_shoff + ehdr.e_shentsize * ehdr.e_shstrndx : ((Elf32_Ehdr*)&ehdr)->e_shoff + ((Elf32_Ehdr*)&ehdr)->e_shentsize * ((Elf32_Ehdr*)&ehdr)->e_shstrndx, SEEK_SET);
    if (fread(is_64bit ? (void*)&strtab_section_header : (void*)&strtab_section_header32, 1, is_64bit ? sizeof(strtab_section_header) : sizeof(strtab_section_header32), file) != (is_64bit ? sizeof(strtab_section_header) : sizeof(strtab_section_header32))) {
        fprintf(stderr, "Error reading section header\n");
        return -1;
    }

    char *shstrtab = malloc(is_64bit ? strtab_section_header.sh_size : strtab_section_header32.sh_size);
    if (!shstrtab) {
        fprintf(stderr, "Memory allocation error\n");
        return -1;
    }

    fseek(file, is_64bit ? strtab_section_header.sh_offset : strtab_section_header32.sh_offset, SEEK_SET);
    if (fread(shstrtab, 1, is_64bit ? strtab_section_header.sh_size : strtab_section_header32.sh_size, file) != (is_64bit ? strtab_section_header.sh_size : strtab_section_header32.sh_size)) {
        fprintf(stderr, "Error reading section header\n");
        return -1;
    }

    int num_symbols = 0;
    SymbolEntry symbols[MAX_SYMBOLS];

    fseek(file, is_64bit ? ehdr.e_shoff : ((Elf32_Ehdr*)&ehdr)->e_shoff, SEEK_SET);
    for (int i = 0; i < (is_64bit ? ehdr.e_shnum : ((Elf32_Ehdr*)&ehdr)->e_shnum); ++i) {
        if (fread(is_64bit ? (void*)&shdr : (void*)&shdr32, 1, is_64bit ? sizeof(shdr) : sizeof(shdr32), file) != (is_64bit ? sizeof(shdr) : sizeof(shdr32))) {
            fprintf(stderr, "Error reading section header\n");
            return -1;
        }

        if ((is_64bit ? shdr.sh_type : shdr32.sh_type) == SHT_SYMTAB || (is_64bit ? shdr.sh_type : shdr32.sh_type) == SHT_DYNSYM) {
            int symbol_count = (is_64bit ? shdr.sh_size : shdr32.sh_size) / (is_64bit ? sizeof(Elf64_Sym) : sizeof(Elf32_Sym));
            fseek(file, is_64bit ? shdr.sh_offset : shdr32.sh_offset, SEEK_SET);

            for (int j = 0; j < symbol_count; ++j) {
                if (is_64bit) {
                    Elf64_Sym sym;
                    if (fread(&sym, 1, sizeof(sym), file) != sizeof(sym)) {
                        fprintf(stderr, "Error reading symbol\n");
                        return -1;
                    }
                    if (sym.st_name == 0) continue;
                    if (num_symbols < MAX_SYMBOLS) {
                        symbols[num_symbols].sym64 = sym;
                        symbols[num_symbols].name = strdup(shstrtab + sym.st_name);
                        symbols[num_symbols].is_64bit = 1;
                        num_symbols++;
                    }
                } else {
                    Elf32_Sym sym;
                    if (fread(&sym, 1, sizeof(sym), file) != sizeof(sym)) {
                        fprintf(stderr, "Error reading symbol\n");
                        return -1;
                    }
                    if (sym.st_name == 0) continue;
                    if (num_symbols < MAX_SYMBOLS) {
                        symbols[num_symbols].sym32 = sym;
                        symbols[num_symbols].name = strdup(shstrtab + sym.st_name);
                        symbols[num_symbols].is_64bit = 0;
                        num_symbols++;
                    }
                }
            }
        }
    }

    qsort(symbols, num_symbols, sizeof(SymbolEntry), compare_symbols);

    for (int i = 0; i < num_symbols; ++i) {
        if (symbols[i].is_64bit) {
            printf("%016lx %s %s\n", (unsigned long)symbols[i].sym64.st_value, get_symbol_type_64(&symbols[i].sym64), symbols[i].name);
        } else {
            printf("%08lx %s %s\n", (unsigned long)symbols[i].sym32.st_value, get_symbol_type_32(&symbols[i].sym32), symbols[i].name);
        }
        free(symbols[i].name);
    }

    free(shstrtab);
    return 0;
}

int process_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("fopen");
        return -1;
    }

    int ret = parse_symbols(file);

    fclose(file);
    return ret;
}
