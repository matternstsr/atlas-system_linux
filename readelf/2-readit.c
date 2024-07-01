#include "2-hreadelf.h"

/**
 * read_elf_section_headers - Reads ELF section headers into memory.
 * @elf_header: Pointer to the ELF header structure.
 * @fd: File descriptor of the ELF file to read from.
 */
void read_elf_section_headers(elf_t *elf_header, int fd)
{
    size_t num_sections = EGET(e_shnum);
    size_t read_size = EGET(e_shentsize) * num_sections;
    char *headers_buffer;

    // If there are no sections, return early
    if (num_sections == 0)
        return;

    // Allocate memory for the section headers
    headers_buffer = calloc(num_sections, read_size);
    if (!headers_buffer) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Seek to the section headers offset in the file
    if (lseek(fd, EGET(e_shoff), SEEK_SET) == -1) {
        perror("Failed to seek to section headers");
        exit(EXIT_FAILURE);
    }

    // Read section headers from the file into the allocated buffer
    size_t bytes_read = read(fd, headers_buffer, read_size);
    if (bytes_read < read_size) {
        perror("Failed to read section headers");
        exit(EXIT_FAILURE);
    }

    // Assign the section headers buffer to the appropriate field in the ELF header structure
    if (IS_32(elf_header->e64)) {
        elf_header->s32 = (void *)headers_buffer;
    } else {
        elf_header->s64 = (void *)headers_buffer;
    }
}

/**
 * read_elf_string_table - Reads the string table from an ELF file.
 * @elf_header: Pointer to the ELF header structure.
 * @fd: File descriptor of the ELF file to read from.
 * Return: Pointer to the beginning of the string table.
 */
char *read_elf_string_table(elf_t *elf_header, int fd)
{
    char *str_table;

    // Allocate memory for the string table
    str_table = calloc(1, SGET(EGET(e_shstrndx), sh_size));
    if (!str_table) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Seek to the string table offset in the file
    if (lseek(fd, SGET(EGET(e_shstrndx), sh_offset), SEEK_SET) == -1) {
        perror("Failed to seek to string table");
        exit(EXIT_FAILURE);
    }

    // Read the string table from the file into the allocated buffer
    if (read(fd, str_table, SGET(EGET(e_shstrndx), sh_size)) < 0) {
        perror("Failed to read string table");
        exit(EXIT_FAILURE);
    }

    return str_table;
}

/**
 * print_elf_section_headers_32 - Prints 32-bit section headers of an ELF file.
 * @elf_header: Pointer to the ELF header structure.
 * @string_table: Pointer to the string table section.
 */
void print_elf_section_headers_32(elf_t *elf_header, char *string_table)
{
    size_t i;

    printf("=== 32-bit Section Headers ===\n");
    for (i = 0; i < EGET(e_shnum); i++) {
        printf("Section %u:\n", (unsigned int)i);
        printf("  Name: %s\n", string_table + SGET(i, sh_name));
        printf("  Type: %s\n", get_section_type_name(SGET(i, sh_type)));
        printf("  Address: 0x%lx\n", SGET(i, sh_addr));
        printf("  Offset: 0x%lx\n", SGET(i, sh_offset));
        printf("  Size: 0x%lx\n", SGET(i, sh_size));
        printf("  Entry Size: 0x%lx\n", SGET(i, sh_entsize));
        printf("  Flags: %s\n", translate_section_flags(elf_header, i));
        printf("  Link: %u\n", SGET(i, sh_link));
        printf("  Info: %u\n", SGET(i, sh_info));
        printf("  Address Alignment: %lu\n", SGET(i, sh_addralign));
    }
}


/**
 * print_elf_section_headers_64 - Prints 64-bit section headers of an ELF file.
 * @elf_header: Pointer to the ELF header structure.
 * @string_table: Pointer to the string table section.
 */
void print_elf_section_headers_64(elf_t *elf_header, char *string_table)
{
    size_t i;

    printf("=== 64-bit Section Headers ===\n");
    for (i = 0; i < EGET(e_shnum); i++) {
        printf("Section %u:\n", (unsigned int)i);
        printf("  Name: %s\n", string_table + SGET(i, sh_name));
        printf("  Type: %s\n", get_section_type_name(SGET(i, sh_type)));
        printf("  Address: 0x%lx\n", SGET(i, sh_addr));
        printf("  Offset: 0x%lx\n", SGET(i, sh_offset));
        printf("  Size: 0x%lx\n", SGET(i, sh_size));
        printf("  Entry Size: 0x%lx\n", SGET(i, sh_entsize));
        printf("  Flags: %s\n", translate_section_flags(elf_header, i));
        printf("  Link: %u\n", SGET(i, sh_link));
        printf("  Info: %u\n", SGET(i, sh_info));
        printf("  Address Alignment: %lu\n", SGET(i, sh_addralign));
    }
}