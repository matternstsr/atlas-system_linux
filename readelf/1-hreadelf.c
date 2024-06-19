#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <byteswap.h>
#include <sys/mman.h>
#include <stddef.h>


struct elf_section_header {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_flags;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
};

void print_section_headers(const void *mem, size_t size, size_t sh_offset, size_t sh_size, size_t sh_entsize, int is_little_endian) {
    // Cast unused parameters to void to avoid compiler errors
    (void)size;
    (void)is_little_endian;

    const char *strtab = (const char *)mem + sh_offset;
    const size_t num_sections = sh_size / sh_entsize;

    printf("There are %zu section headers, starting at offset 0x%zx:\n\n", num_sections, sh_offset);

    printf("Section Headers:\n");
    printf("  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n");

    for (size_t i = 0; i < num_sections; ++i) {
        const struct elf_section_header *hdr = (const struct elf_section_header *)((const char *)mem + sh_offset + i * sh_entsize);

        printf("  [%2zu] %-17s ", i, strtab + hdr->sh_name);

        switch (hdr->sh_type) {
            case SHT_NULL:          printf("NULL            "); break;
            case SHT_PROGBITS:      printf("PROGBITS        "); break;
            case SHT_NOTE:          printf("NOTE            "); break;
            case SHT_NOBITS:        printf("NOBITS          "); break;
            case SHT_HASH:          printf("HASH            "); break;
            case SHT_DYNAMIC:       printf("DYNAMIC         "); break;
            case SHT_REL:           printf("REL             "); break;
            case SHT_RELA:          printf("RELA            "); break;
            case SHT_SYMTAB:        printf("SYMTAB          "); break;
            case SHT_STRTAB:        printf("STRTAB          "); break;
            case SHT_DYNSYM:        printf("DYNSYM          "); break;
            case SHT_INIT_ARRAY:    printf("INIT_ARRAY      "); break;
            case SHT_FINI_ARRAY:    printf("FINI_ARRAY      "); break;
            case SHT_PREINIT_ARRAY: printf("PREINIT_ARRAY   "); break;
            case SHT_GROUP:         printf("GROUP           "); break;
            case SHT_SYMTAB_SHNDX:  printf("SYMTAB SECTION INDICES"); break;
            default:                printf("                ");
        }

        printf("%016lx ", (unsigned long)hdr->sh_addr);
        printf("%06lx ", (unsigned long)hdr->sh_offset);
        printf("%06lx ", (unsigned long)hdr->sh_size);
        printf("%2lx ", (unsigned long)hdr->sh_entsize);
        printf("%c%c%c%c%c%c%c%c ",
               hdr->sh_flags & SHF_WRITE    ? 'W' : ' ',
               hdr->sh_flags & SHF_ALLOC    ? 'A' : ' ',
               hdr->sh_flags & SHF_EXECINSTR ? 'X' : ' ',
               hdr->sh_flags & SHF_MERGE    ? 'M' : ' ',
               hdr->sh_flags & SHF_STRINGS  ? 'S' : ' ',
               hdr->sh_flags & SHF_INFO_LINK ? 'I' : ' ',
               hdr->sh_flags & SHF_LINK_ORDER ? 'L' : ' ',
               hdr->sh_flags & SHF_OS_NONCONFORMING ? 'O' : ' ');

        printf("%3u ", hdr->sh_link);
        printf("%3u ", hdr->sh_info);
        printf("%2lu\n", hdr->sh_addralign);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return EXIT_FAILURE;
    }

    void *mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return EXIT_FAILURE;
    }

    // Read ELF header
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)mem;
    if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not an ELF file.\n");
        munmap(mem, st.st_size);
        close(fd);
        return EXIT_FAILURE;
    }

    size_t sh_offset = ehdr->e_shoff;
    size_t sh_size = ehdr->e_shentsize * ehdr->e_shnum;
    size_t sh_entsize = ehdr->e_shentsize;

    // Determine endianness
    int is_little_endian;
    if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB) {
        is_little_endian = 1;
    } else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB) {
        is_little_endian = 0;
    } else {
        fprintf(stderr, "Unknown endianness.\n");
        munmap(mem, st.st_size);
        close(fd);
        return EXIT_FAILURE;
    }

    // Print section headers
    print_section_headers(mem, st.st_size, sh_offset, sh_size, sh_entsize, is_little_endian);

    munmap(mem, st.st_size);
    close(fd);

    return EXIT_SUCCESS;
}
