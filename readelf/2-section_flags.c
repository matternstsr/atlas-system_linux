#include "2-hreadelf.h"

/**
* @brief Translates section flags into a string representation.
*
* @param EH Pointer to the ELF header structure.
* @param section_index Index of the section whose flags to translate.
* @return char* String representation of section flags based on legend.
*/
char *translate_section_flags(elf_t *EH, size_t section_index)
{
	static char flags_string[32];
	unsigned long flags = SGET(section_index, sh_flags);
	char *p = flags_string;

	/* Translate each flag into its corresponding character */
	if (flags & SHF_WRITE)
	*p++ = 'W';
	if (flags & SHF_ALLOC)
	*p++ = 'A';
	if (flags & SHF_EXECINSTR)
	*p++ = 'X';
	if (flags & SHF_MERGE)
	*p++ = 'M';
	if (flags & SHF_STRINGS)
	*p++ = 'S';
	if (flags & SHF_INFO_LINK)
	*p++ = 'I';
	if (flags & SHF_LINK_ORDER)
	*p++ = 'L';
	if (flags & SHF_OS_NONCONFORMING)
	*p++ = 'O';
	if (flags & SHF_GROUP)
	*p++ = 'G';
	if (flags & SHF_TLS)
	*p++ = 'T';
	if (flags & SHF_EXCLUDE)
	*p++ = 'E';

	/* Null-terminate the string */
	*p = '\0';

	return (flags_string);
}
