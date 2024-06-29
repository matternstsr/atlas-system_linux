#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <elf.h>

void print_program_headers(FILE *file) {
    Elf64_Ehdr elf_header;
    Elf64_Phdr *program_headers;
    int i;

    // Read ELF header
    fread(&elf_header, 1, sizeof(Elf64_Ehdr), file);

    // Validate ELF magic number
    if (memcmp(elf_header.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Error: Not an ELF file\n");
        exit(1);
    }

    // Read program headers
    fseek(file, elf_header.e_phoff, SEEK_SET);
    program_headers = malloc(elf_header.e_phnum * sizeof(Elf64_Phdr));
    fread(program_headers, sizeof(Elf64_Phdr), elf_header.e_phnum, file);

    // Print ELF information
    printf("Elf file type is %s\n", elf_header.e_type == ET_EXEC ? "EXEC (Executable file)" :
                                    (elf_header.e_type == ET_DYN ? "DYN (Shared object file)" :
                                    (elf_header.e_type == ET_REL ? "REL (Relocatable file)" : "Unknown")));
    printf("Entry point 0x%lx\n", (unsigned long)elf_header.e_entry);
    printf("There are %d program headers, starting at offset %lu\n\n", elf_header.e_phnum, (unsigned long)elf_header.e_phoff);

    // Print program headers
    printf("Program Headers:\n");
    printf("  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align\n");
    for (i = 0; i < elf_header.e_phnum; ++i) {
        printf("  %-14s 0x%06lx 0x%016lx 0x%016lx 0x%06lx 0x%06lx %-3s %lx\n",
            program_headers[i].p_type == PT_NULL ? "NULL" :
            (program_headers[i].p_type == PT_LOAD ? "LOAD" :
            (program_headers[i].p_type == PT_DYNAMIC ? "DYNAMIC" :
            (program_headers[i].p_type == PT_INTERP ? "INTERP" :
            (program_headers[i].p_type == PT_NOTE ? "NOTE" :
            (program_headers[i].p_type == PT_PHDR ? "PHDR" :
            (program_headers[i].p_type == PT_TLS ? "TLS" : "UNKNOWN")))))),
            (unsigned long)program_headers[i].p_offset,
            (unsigned long)program_headers[i].p_vaddr,
            (unsigned long)program_headers[i].p_paddr,
            (unsigned long)program_headers[i].p_filesz,
            (unsigned long)program_headers[i].p_memsz,
            (program_headers[i].p_flags & PF_R ? "R" : " "),
            (unsigned long)program_headers[i].p_align);
    }

    // Cleanup
    free(program_headers);
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

    print_program_headers(file);

    fclose(file);
    return 0;
}
