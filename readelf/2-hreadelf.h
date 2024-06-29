#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <elf.h>

void print_program_headers(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    Elf64_Ehdr elf_header;
    fread(&elf_header, 1, sizeof(Elf64_Ehdr), file);

    // Validate ELF magic number
    if (memcmp(elf_header.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Error: Not an ELF file\n");
        fclose(file);
        return;
    }

    // Print ELF file type
    const char *elf_type_str = "Unknown";
    switch (elf_header.e_type) {
        case ET_NONE:   elf_type_str = "NONE (Unknown type)"; break;
        case ET_REL:    elf_type_str = "REL (Relocatable file)"; break;
        case ET_EXEC:   elf_type_str = "EXEC (Executable file)"; break;
        case ET_DYN:    elf_type_str = "DYN (Shared object file)"; break;
        case ET_CORE:   elf_type_str = "CORE (Core file)"; break;
        default:        break;
    }
    printf("Elf file type is %s\n", elf_type_str);

    // Print entry point address
    printf("Entry point 0x%lx\n", (unsigned long)elf_header.e_entry);

    // Print number of program headers and their starting offset
    printf("There are %d program headers, starting at offset %lu\n\n",
            elf_header.e_phnum, (unsigned long)elf_header.e_phoff);

    // Seek to the program header offset
    fseek(file, elf_header.e_phoff, SEEK_SET);

    // Read and print program headers
    Elf64_Phdr program_header;
    printf("Program Headers:\n");
    printf("  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align\n");
    for (int i = 0; i < elf_header.e_phnum; ++i) {
        fread(&program_header, 1, sizeof(Elf64_Phdr), file);
        
        // Adjust printing to match desired format and filter to only desired program headers
        if (program_header.p_type == PT_LOAD || program_header.p_type == PT_INTERP || program_header.p_type == PT_DYNAMIC || program_header.p_type == PT_NOTE || program_header.p_type == PT_PHDR) {
            printf("  %-14s 0x%06lx 0x%016lx 0x%016lx 0x%06lx 0x%06lx %-3s %lx\n",
                program_header.p_type == PT_NULL ? "NULL" :
                (program_header.p_type == PT_LOAD ? "LOAD" :
                (program_header.p_type == PT_DYNAMIC ? "DYNAMIC" :
                (program_header.p_type == PT_INTERP ? "INTERP" :
                (program_header.p_type == PT_NOTE ? "NOTE" :
                (program_header.p_type == PT_PHDR ? "PHDR" : "UNKNOWN"))))),
                (unsigned long)program_header.p_offset,
                (unsigned long)program_header.p_vaddr,
                (unsigned long)program_header.p_paddr,
                (unsigned long)program_header.p_filesz,
                (unsigned long)program_header.p_memsz,
                (program_header.p_flags & PF_R ? "R" : " "),
                (unsigned long)program_header.p_align);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        return 1;
    }

    print_program_headers(argv[1]);

    return 0;
}
