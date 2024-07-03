#include "2-hreadelf.h"

/**
* display_all_elf_program_headers - displays all the ELF program headers
* @EH: pointer to the ELF header struct
* @fd: file descriptor of the ELF file
* Return: 0 on success, otherwise exit_status
*/
int display_all_elf_program_headers(elf_t *EH, int fd)
{
	char *section_strings;
	size_t i;

	if (!EGET(e_phnum))
	{
		printf("\nThere are no program headers in this file.\n");
		return (0);
	}

	printf(PROGRAM_TITLE, get_elf_file_type(EH), EGET(e_entry),
		EGET(e_phnum), EGET(e_phoff));

	read_elf_section_headers(EH, fd);
	read_program_headers_from_file(EH, fd);

	for (i = 0; i < EGET(e_shnum); i++)
		switch_endianness_sections(EH, i);

	for (i = 0; i < EGET(e_phnum); i++)
		switch_endianness_program_headers(EH, i);

	section_strings = read_elf_string_table(EH, fd);

	printf("\nProgram Headers:\n");
	if (IS_32(EH->e64))
		print_program_headers_32bit(EH, section_strings, fd);
	else
		print_program_headers_64bit(EH, section_strings, fd);

	print_section_segment_mappings(EH, section_strings);

	free(section_strings);
	return (0);
}
