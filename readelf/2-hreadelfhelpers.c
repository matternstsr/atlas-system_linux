#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

/* Function prototypes */
void solaris32_func(const char *filename);
void sortix32_func(const char *filename);
void sparcbigendian32_func(const char *filename);
void jpeg_mod_func(const char *filename);
void case_python_obj_func(const char *filename);
void vgpreload_memcheck_x86_linux_so_func(const char *filename);

const char *section_type_to_string_func(unsigned int type);
const char *flags_to_string_func(unsigned long flags);
void readelf_sections_func(FILE *fp);

/* Function definitions */

/* Function to handle solaris32 ELF files */
void solaris32_func(const char *filename) {
    printf("Handling solaris32 ELF file: %s\n", filename);
}

/* Function to handle sortix32 ELF files */
void sortix32_func(const char *filename) {
    printf("Handling sortix32 ELF file: %s\n", filename);
}

/* Function to handle sparcbigendian32 ELF files */
void sparcbigendian32_func(const char *filename) {
    printf("Handling sparcbigendian32 ELF file: %s\n", filename);
}

/* Function to handle jpeg.mod files */
void jpeg_mod_func(const char *filename) {
    printf("Handling jpeg.mod file: %s\n", filename);
}

/* Function to handle case: python.obj files */
void case_python_obj_func(const char *filename) {
    printf("Handling case: python.obj file: %s\n", filename);
}

/* Function to handle vgpreload_memcheck-x86-linux.so files */
void vgpreload_memcheck_x86_linux_so_func(const char *filename) {
    printf("Handling vgpreload_memcheck-x86-linux.so file: %s\n", filename);
}

/* Function to map section type to string name */
const char *section_type_to_string_func(unsigned int type) {
    const char *type_str = "UNKNOWN";

    switch (type) {
        case SHT_NULL:            type_str = "NULL";            break;
        case SHT_PROGBITS:        type_str = "PROGBITS";        break;
        case SHT_SYMTAB:          type_str = "SYMTAB";          break;
        case SHT_STRTAB:          type_str = "STRTAB";          break;
        case SHT_RELA:            type_str = "RELA";            break;
        case SHT_HASH:            type_str = "HASH";            break;
        case SHT_DYNAMIC:         type_str = "DYNAMIC";         break;
        case SHT_NOTE:            type_str = "NOTE";            break;
        case SHT_NOBITS:          type_str = "NOBITS";          break;
        case SHT_REL:             type_str = "REL";             break;
        case SHT_SHLIB:           type_str = "SHLIB";           break;
        case SHT_DYNSYM:          type_str = "DYNSYM";          break;
        case SHT_INIT_ARRAY:      type_str = "INIT_ARRAY";      break;
        case SHT_FINI_ARRAY:      type_str = "FINI_ARRAY";      break;
        case SHT_PREINIT_ARRAY:   type_str = "PREINIT_ARRAY";   break;
        case SHT_GROUP:           type_str = "GROUP";           break;
        case SHT_SYMTAB_SHNDX:    type_str = "SYMTAB_SHNDX";    break;
        case SHT_GNU_HASH:        type_str = "GNU_HASH";        break;
        case SHT_GNU_verdef:      type_str = "GNU_verdef";      break;
        case SHT_GNU_verneed:     type_str = "VERNEED";         break;
        case SHT_GNU_versym:      type_str = "VERSYM";          break;
        case SHT_SUNW_move:       type_str = "SUNW_move";       break;
        case SHT_SUNW_COMDAT:     type_str = "SUNW_COMDAT";     break;
        case SHT_SUNW_syminfo:    type_str = "SUNW_syminfo";    break;
        case SHT_LOPROC:          type_str = "LOPROC";          break;
        case SHT_HIPROC:          type_str = "HIPROC";          break;
        case SHT_LOUSER:          type_str = "LOUSER";          break;
        case SHT_HIUSER:          type_str = "HIUSER";          break;
        default:                  type_str = "UNKNOWN";         break;
    }

    return type_str;
}

/* Function to convert flags to string representation */
const char *flags_to_string_func(unsigned long flags) {
    static char buf[9];
    int pos = 0;

    if (flags & SHF_WRITE)    buf[pos++] = 'W';
    if (flags & SHF_ALLOC)    buf[pos++] = 'A';
    if (flags & SHF_EXECINSTR) buf[pos++] = 'X';
    if (flags & SHF_MERGE)    buf[pos++] = 'M';
    if (flags & SHF_STRINGS)  buf[pos++] = 'S';
    if (flags & SHF_INFO_LINK) buf[pos++] = 'I';
    if (flags & SHF_LINK_ORDER) buf[pos++] = 'L';
    
    buf[pos] = '\0';

    return buf;
}

/* Function to read and display section headers */
void readelf_sections_func(FILE *fp) {
    Elf64_Ehdr ehdr;
    Elf64_Shdr *shdr = NULL;
    Elf64_Shdr shstrtab_header;
    char *shstrtab = NULL;
    size_t shdr_size;
    long shoff;
    int i;

    /* Read ELF header */
    fread(&ehdr, sizeof(Elf64_Ehdr), 1, fp);

    /* Seek to section header table offset */
    shoff = ehdr.e_shoff;
    fseek(fp, shoff, SEEK_SET);

    /* Allocate memory for section headers */
    shdr_size = ehdr.e_shentsize * ehdr.e_shnum;
    shdr = malloc(shdr_size);
    if (shdr == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for section headers\n");
        return;
    }

    /* Read section headers */
    fread(shdr, ehdr.e_shentsize, ehdr.e_shnum, fp);

    /* Read .shstrtab section header */
    shstrtab_header = shdr[ehdr.e_shstrndx];
    shstrtab = malloc(shstrtab_header.sh_size);
    if (shstrtab == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for section header string table\n");
        free(shdr);
        return;
    }

    /* Read .shstrtab section content */
    fseek(fp, shstrtab_header.sh_offset, SEEK_SET);
    fread(shstrtab, 1, shstrtab_header.sh_size, fp);

    /* Print section headers */
    printf("There are %d section headers, starting at offset 0x%lx:\n\n", ehdr.e_shnum, shoff);
    printf("Section Headers:\n");
    printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
    for (i = 0; i < ehdr.e_shnum; i++) {
        const char *name = &shstrtab[shdr[i].sh_name];

        printf("  [%2d] %-17s %-15s %08lx %06lx %06lx %02lx %3s %2d %3d %2d\n",
               i, name, section_type_to_string_func(shdr[i].sh_type),
               (unsigned long)shdr[i].sh_addr, (unsigned long)shdr[i].sh_offset,
               (unsigned long)shdr[i].sh_size, (unsigned long)shdr[i].sh_entsize,
               flags_to_string_func(shdr[i].sh_flags), (int)shdr[i].sh_link,
               (int)shdr[i].sh_info, (int)shdr[i].sh_addralign);
    }

    /* Print Key to Flags */
    printf("\nKey to Flags:\n");
    printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
    printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
    printf("  O (extra OS processing required), o (OS specific), p (processor specific)\n");

    free(shdr);
    free(shstrtab);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ELF filename>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    readelf_sections_func(fp);

    fclose(fp);
    return 0;
}
