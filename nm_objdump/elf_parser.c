#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <elf.h>

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

static char get_symbol_binding(uint8_t info) {
    switch (ELF32_ST_BIND(info)) {
        case STB_LOCAL:  return 'l';
        case STB_GLOBAL: return 'g';
        case STB_WEAK:   return 'w';
        default:         return '?';
    }
}

static const char *get_symbol_type_32(Elf32_Sym *sym) {
    switch (ELF32_ST_TYPE(sym->st_info)) {
        case STT_NOTYPE: return "";
        case STT_OBJECT: return "d";
        case STT_FUNC:   return "t";
        case STT_SECTION:return "n";
        case STT_FILE:   return "f";
        default:         return "?";
    }
}

static const char *get_symbol_type_64(Elf64_Sym *sym) {
    switch (ELF64_ST_TYPE(sym->st_info)) {
        case STT_NOTYPE: return "";
        case STT_OBJECT: return "d";
        case STT_FUNC:   return "t";
        case STT_SECTION:return "n";
        case STT_FILE:   return "f";
        default:         return "?";
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

    uint16_t shnum = is_64bit ? ehdr.e_shnum : ((Elf32_Ehdr*)&ehdr)->e_shnum;
    uint16_t shentsize = is_64bit ? ehdr.e_shentsize : ((Elf32_Ehdr*)&ehdr)->e_shentsize;
    uint64_t shoff = is_64bit ? ehdr.e_shoff : ((Elf32_Ehdr*)&ehdr)->e_shoff;

    Elf64_Shdr shdr;
    Elf32_Shdr shdr32;

    fseek(file, shoff, SEEK_SET);

    char *section_headers_strtab = NULL;

    for (int i = 0; i < shnum; ++i) {
        if (fread(is_64bit ? (void*)&shdr : (void*)&shdr32, 1, shentsize, file) != shentsize) {
            fprintf(stderr, "Error reading section header\n");
            return -1;
        }

        if (is_64bit && shdr.sh_type == SHT_STRTAB) {
            if (shdr.sh_flags & SHF_ALLOC) {
                section_headers_strtab = malloc(shdr.sh_size);
                if (!section_headers_strtab) {
                    fprintf(stderr, "Memory allocation error\n");
                    return -1;
                }
                fseek(file, shdr.sh_offset, SEEK_SET);
                if (fread(section_headers_strtab, 1, shdr.sh_size, file) != shdr.sh_size) {
                    fprintf(stderr, "Error reading section header string table\n");
                    return -1;
                }
            }
        } else if (!is_64bit && shdr32.sh_type == SHT_STRTAB) {
            if (shdr32.sh_flags & SHF_ALLOC) {
                section_headers_strtab = malloc(shdr32.sh_size);
                if (!section_headers_strtab) {
                    fprintf(stderr, "Memory allocation error\n");
                    return -1;
                }
                fseek(file, shdr32.sh_offset, SEEK_SET);
                if (fread(section_headers_strtab, 1, shdr32.sh_size, file) != shdr32.sh_size) {
                    fprintf(stderr, "Error reading section header string table\n");
                    return -1;
                }
            }
        }
    }

    int num_symbols = 0;
    SymbolEntry symbols[MAX_SYMBOLS];

    fseek(file, shoff, SEEK_SET);
    for (int i = 0; i < shnum; ++i) {
        if (fread(is_64bit ? (void*)&shdr : (void*)&shdr32, 1, shentsize, file) != shentsize) {
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
                        symbols[num_symbols].name = strdup(section_headers_strtab + sym.st_name);
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
                        symbols[num_symbols].name = strdup(section_headers_strtab + sym.st_name);
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
            printf("%016lx %c %s\n", (unsigned long)symbols[i].sym64.st_value, get_symbol_binding(symbols[i].sym64.st_info), symbols[i].name);
        } else {
            printf("%08lx %c %s\n", (unsigned long)symbols[i].sym32.st_value, get_symbol_binding(symbols[i].sym32.st_info), symbols[i].name);
        }
        free(symbols[i].name);
    }

    free(section_headers_strtab);
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
