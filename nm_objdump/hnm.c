#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <elf.h>
#include "elf_parser.h"
/* #include <libelf.h> */

#define MAX_SYMBOLS 1024

typedef struct {
    Elf64_Sym sym;
    char *name;
} SymbolEntry;

static int compare_symbols(const void *a, const void *b) {
    const SymbolEntry *sa = (const SymbolEntry *)a;
    const SymbolEntry *sb = (const SymbolEntry *)b;
    return strcmp(sa->name, sb->name);
}

static const char *get_symbol_type(Elf64_Sym *sym) {
    switch (ELF64_ST_TYPE(sym->st_info)) {
        case STT_NOTYPE:
            return " ";
        case STT_OBJECT:
            return "B";
        case STT_FUNC:
            return "T";
        case STT_SECTION:
            return "S";
        case STT_FILE:
            return "f";
        case STT_COMMON:
            return "C";
        case STT_TLS:
            return "T";
        case STT_GNU_IFUNC:
            return "i";
        default:
            return "?";
    }
}
