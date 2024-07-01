#include "2-hreadelf.h"

/**
 * display_all_elf_program_headers - displays all the ELF program headers
 * @elf_header: pointer to the ELF header struct
 * @fd: file descriptor of the ELF file
 * Return: 0 on success, otherwise exit_status
 */
int display_all_elf_program_headers(elf_t *elf_header, int fd)
{
    char *section_strings;
    size_t i;

    if (!EGET(e_phnum))
    {
        printf("\nThere are no program headers in this file.\n");
        return 0;
    }

    printf(PROGRAM_TITLE, get_elf_file_type(elf_header), EGET(e_entry),
           EGET(e_phnum), EGET(e_phoff));

    read_elf_section_headers(elf_header, fd);
    read_program_headers_from_file(elf_header, fd);

    for (i = 0; i < EGET(e_shnum); i++)
        switch_endianness_sections(elf_header, i);

    for (i = 0; i < EGET(e_phnum); i++)
        switch_endianness_program_headers(elf_header, i);

    section_strings = read_elf_string_table(elf_header, fd);

    printf("\nProgram Headers:\n");
    if (IS_32(elf_header->e64))
        print_program_headers_32bit(elf_header, section_strings, fd);
    else
        print_program_headers_64bit(elf_header, section_strings, fd);

    print_section_segment_mappings(elf_header, section_strings);

    free(section_strings);
    return 0;
}
