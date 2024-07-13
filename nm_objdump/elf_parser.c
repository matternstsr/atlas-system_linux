#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <elf.h>

#define MAX_SYMBOLS 1024

#if defined(ELFCLASS32)
typedef Elf32_Sym Elf_Sym;
typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Shdr Elf_Shdr;
#define ELF_ST_TYPE ELF32_ST_TYPE
#define ELF_CLASS ELFCLASS32
#elif defined(ELFCLASS64)
typedef Elf64_Sym Elf_Sym;
typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Shdr Elf_Shdr;
#define ELF_ST_TYPE ELF64_ST_TYPE
#define ELF_CLASS ELFCLASS64
#else
#error "[stderr]: Unsupported ELF class"
#endif

typedef struct {
    Elf_Sym sym;
    char *name;
} SymbolEntry;

static const char *get_symbol_type(const Elf_Sym *sym);
static int compare_symbols(const void *a, const void *b);
static int parse_symbols(FILE *file);

static const char *get_symbol_type(const Elf_Sym *sym)
{
    switch (ELF_ST_TYPE(sym->st_info))
    {
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

static int compare_symbols(const void *a, const void *b)
{
    const SymbolEntry *sa = (const SymbolEntry *)a;
    const SymbolEntry *sb = (const SymbolEntry *)b;
    if (sa->sym.st_value < sb->sym.st_value)
        return -1;
    else if (sa->sym.st_value > sb->sym.st_value)
        return 1;
    else
        return 0;
}

static int parse_symbols(FILE *file)
{
    Elf_Ehdr ehdr;
    Elf_Shdr shdr;
    Elf_Shdr strtab_section_header;
    char *shstrtab = NULL;
    SymbolEntry symbols[MAX_SYMBOLS];
    int num_symbols = 0, i, j, symbol_count;

    if (fread(&ehdr, 1, sizeof(ehdr), file) != sizeof(ehdr))
    {
        fprintf(stderr, "[stderr]: Error reading ELF header\n");
        return -1;
    }

    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0)
    {
        fprintf(stderr, "[stderr]: Not an ELF file\n");
        return -1;
    }

    if (ehdr.e_ident[EI_CLASS] != ELF_CLASS)
    {
        fprintf(stderr, "[stderr]: Unsupported ELF class\n");
        return -1;
    }

    fseek(file, ehdr.e_shoff, SEEK_SET);
    if (fread(&shdr, 1, sizeof(shdr), file) != sizeof(shdr))
    {
        fprintf(stderr, "[stderr]: Error reading section header\n");
        return -1;
    }

    fseek(file, ehdr.e_shoff + ehdr.e_shentsize * ehdr.e_shstrndx, SEEK_SET);
    if (fread(&strtab_section_header, 1, sizeof(strtab_section_header), file) !=
        sizeof(strtab_section_header))
    {
        fprintf(stderr, "[stderr]: Error reading section header\n");
        return -1;
    }

    shstrtab = (char *)malloc(strtab_section_header.sh_size);
    if (!shstrtab)
    {
        fprintf(stderr, "[stderr]: Memory allocation error\n");
        return -1;
    }

    fseek(file, strtab_section_header.sh_offset, SEEK_SET);
    if (fread(shstrtab, 1, strtab_section_header.sh_size, file) != strtab_section_header.sh_size)
    {
        fprintf(stderr, "[stderr]: Error reading section header\n");
        free(shstrtab);
        return -1;
    }

    fseek(file, ehdr.e_shoff, SEEK_SET);
    for (i = 0; i < ehdr.e_shnum; ++i)
    {
        if (fread(&shdr, 1, sizeof(shdr), file) != sizeof(shdr))
        {
            fprintf(stderr, "[stderr]: Error reading section header\n");
            free(shstrtab);
            return -1;
        }

        if (shdr.sh_type == SHT_SYMTAB || shdr.sh_type == SHT_DYNSYM)
        {
            symbol_count = shdr.sh_size / sizeof(Elf_Sym);
            fseek(file, shdr.sh_offset, SEEK_SET);

            for (j = 0; j < symbol_count; ++j)
            {
                Elf_Sym sym;
                if (fread(&sym, 1, sizeof(sym), file) != sizeof(sym))
                {
                    fprintf(stderr, "[stderr]: Error reading symbol\n");
                    free(shstrtab);
                    return -1;
                }

                if (sym.st_name == 0)
                {
                    continue; /* Skip unnamed symbols  */
                }

                if (num_symbols < MAX_SYMBOLS)
                {
                    symbols[num_symbols].sym = sym;
                    symbols[num_symbols].name = strdup(shstrtab + sym.st_name);
                    num_symbols++;
                }
            }
        }
    }

    qsort(symbols, num_symbols, sizeof(SymbolEntry), compare_symbols);

    if (num_symbols == 0)
    {
        fprintf(stderr, "[stderr]: ./hnm: libperl.so.5.18: no symbols\n");
    }
    else
    {
        for (i = 0; i < num_symbols; ++i)
        {
            printf("%016lx %s %s\n", (unsigned long)symbols[i].sym.st_value,
                   get_symbol_type(&symbols[i].sym), symbols[i].name);
            free(symbols[i].name);
        }
    }

    free(shstrtab);
    return 0;
}

int process_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("[stderr]: fopen");
        return -1;
    }

    printf("%s:\n", filename);
    int ret = parse_symbols(file);

    fclose(file);
    return ret;
}
