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
    printf("  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n");

    for (i = 0; i < ehdr.e_shnum; ++i) {
        printf("  [%2d] %-18s %-16s %016lx %06lx %06lx %02lx %c   %2d   %2d   %2ld\n",
            i,
            &shstrtab[shdr[i].sh_name],  /* Section name from string table */
            "",  /* Placeholder for type (to be filled) */
            (unsigned long)shdr[i].sh_addr,
            (unsigned long)shdr[i].sh_offset,
            (unsigned long)shdr[i].sh_size,
            (unsigned long)shdr[i].sh_entsize,
            (shdr[i].sh_flags & SHF_ALLOC) ? 'A' : ' ',
            shdr[i].sh_link,
            shdr[i].sh_info,
            shdr[i].sh_addralign
        );
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
