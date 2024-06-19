#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

void print_section_headers(FILE *file, Elf64_Ehdr ehdr) {
    Elf64_Shdr *shdr;
    Elf64_Shdr *strtab_shdr;
    char *shstrtab;
    int i;

    /* Move to section header table offset */
    fseek(file, ehdr.e_shoff, SEEK_SET);

    /* Read section headers */
    shdr = (Elf64_Shdr *)malloc(ehdr.e_shentsize * ehdr.e_shnum);
    fread(shdr, ehdr.e_shentsize, ehdr.e_shnum, file);

    /* Find string table section header */
    strtab_shdr = &shdr[ehdr.e_shstrndx];

    /* Allocate memory for section header string table */
    shstrtab = (char *)malloc(strtab_shdr->sh_size);
    fseek(file, strtab_shdr->sh_offset, SEEK_SET);
    fread(shstrtab, strtab_shdr->sh_size, 1, file);

    /* Print section headers */
    printf("Section Headers:\n");
    printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");

    for (i = 0; i < ehdr.e_shnum; ++i) {
        const char *name = &shstrtab[shdr[i].sh_name];
        const char *type = "";
        
        switch (shdr[i].sh_type) {
            case SHT_NULL:            type = "NULL";          break;
            case SHT_PROGBITS:        type = "PROGBITS";      break;
            case SHT_SYMTAB:          type = "SYMTAB";        break;
            case SHT_STRTAB:          type = "STRTAB";        break;
            case SHT_RELA:            type = "RELA";          break;
            case SHT_HASH:            type = "HASH";          break;
            case SHT_DYNAMIC:         type = "DYNAMIC";       break;
            case SHT_NOTE:            type = "NOTE";          break;
            case SHT_NOBITS:          type = "NOBITS";        break;
            case SHT_REL:             type = "REL";           break;
            case SHT_SHLIB:           type = "SHLIB";         break;
            case SHT_DYNSYM:          type = "DYNSYM";        break;
            case SHT_INIT_ARRAY:      type = "INIT_ARRAY";    break;
            case SHT_FINI_ARRAY:      type = "FINI_ARRAY";    break;
            case SHT_PREINIT_ARRAY:   type = "PREINIT_ARRAY"; break;
            case SHT_GROUP:           type = "GROUP";         break;
            case SHT_SYMTAB_SHNDX:    type = "SYMTAB_SHNDX";  break;
            case SHT_LOOS:            type = "LOOS";          break;
            case SHT_HIOS:            type = "HIOS";          break;
            case SHT_LOPROC:          type = "LOPROC";        break;
            case SHT_HIPROC:          type = "HIPROC";        break;
            case SHT_LOUSER:          type = "LOUSER";        break;
            case SHT_HIUSER:          type = "HIUSER";        break;
        }

        printf("  [%2d] %-17s %-15s %08lx %06lx %06lx %02lx %c%3d%4d%3ld\n",
            i,
            name,
            type,
            (unsigned long)shdr[i].sh_addr,
            (unsigned long)shdr[i].sh_offset,
            (unsigned long)shdr[i].sh_size,
            (unsigned long)shdr[i].sh_entsize,
            (shdr[i].sh_flags & SHF_ALLOC) ? 'A' : ' ',
            shdr[i].sh_link,
            shdr[i].sh_info,
            shdr[i].sh_addralign
        );

        /* Print Key to Flags after 30 section headers */
        if (i == 29) {
            printf("\nKey to Flags:\n");
            printf("  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)\n");
            printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
            printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n\n");
        }
    }

    free(shdr);
    free(shstrtab);
}

int main(int argc, char *argv[]) {
    FILE *file;
    Elf64_Ehdr ehdr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        return 1;
    }

    file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fread(&ehdr, sizeof(ehdr), 1, file);

    print_section_headers(file, ehdr);

    fclose(file);
    return 0;
}
