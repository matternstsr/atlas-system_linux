#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

void print_section_headers(FILE *file, Elf64_Ehdr ehdr) {
    fseek(file, ehdr.e_shoff, SEEK_SET); 
    Elf64_Shdr *shdr = (Elf64_Shdr *)malloc(ehdr.e_shentsize * ehdr.e_shnum);
    fread(shdr, ehdr.e_shentsize, ehdr.e_shnum, file);
    printf("There are %d section headers, starting at offset 0x%lx:\n\n", ehdr.e_shnum, ehdr.e_shoff);
    printf("Section Headers:\n");
    printf("  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n");

    for (int i = 0; i < ehdr.e_shnum; ++i) {
        printf("  [%2d] %-17s %-15s %016lx %06lx %06lx %02lx %c%c %3d %3d %2ld\n",
            i,
            "", 
            "", 
            (unsigned long)shdr[i].sh_addr,
            (unsigned long)shdr[i].sh_offset,
            (unsigned long)shdr[i].sh_size,
            (unsigned long)shdr[i].sh_entsize,
            (shdr[i].sh_flags & SHF_ALLOC) ? 'A' : ' ',
            (shdr[i].sh_flags & SHF_WRITE) ? 'W' : ' ',
            shdr[i].sh_link,
            shdr[i].sh_info,
            shdr[i].sh_addralign
        );
    }

    free(shdr);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    Elf64_Ehdr ehdr;
    fread(&ehdr, sizeof(ehdr), 1, file);

    print_section_headers(file, ehdr);

    fclose(file);
    return 0;
}
