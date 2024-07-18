#include "hnm.h"

/*
* ef_flags - Prints the list of flags for the ELF file.
* @flags: Flags for the ELF file.
*
* This function prints a list of flags associated with the ELF file.
*/
void ef_flags(unsigned long flags)
{
	int first = 1;/* Flag for first item */
	/* Print flags */
	if (flags & WP_TEXT)
		first = (printf("%s%s", first ? "" : ", ", "WP_TEXT"), 0);
	if (flags & EXEC_P)
		first = (printf("%s%s", first ? "" : ", ", "EXEC_P"), 0);
	if (flags & HAS_LINENO)
		first = (printf("%s%s", first ? "" : ", ", "HAS_LINENO"), 0);
	if (flags & HAS_DEBUG)
		first = (printf("%s%s", first ? "" : ", ", "HAS_DEBUG"), 0);
	if (flags & HAS_SYMS)
		first = (printf("%s%s", first ? "" : ", ", "HAS_SYMS"), 0);
	if (flags & HAS_RELOC)
		first = (printf("%s%s", first ? "" : ", ", "HAS_RELOC"), 0);
	if (flags & DYNAMIC)
		first = (printf("%s%s", first ? "" : ", ", "DYNAMIC"), 0);
	if (flags & D_PAGED)
		first = (printf("%s%s", first ? "" : ", ", "D_PAGED"), 0);
	if (flags & HAS_LOCALS)
		first = (printf("%s%s", first ? "" : ", ", "HAS_LOCALS"), 0);
	if (flags & HAS_RELOC)
	if (!first)
		printf("\n");/* Print newline if flags were printed */
}

/*
* essential_information - Prints the header information for the ELF file.
* @elf_header: The internal ELF header.
* @string_table: The header string table.
*
* This function prints essential header information for the ELF file.
*/
void essential_information(elf_t *elf_header, char *string_table)
{
	unsigned long flags = 0;  /* Flags for ELF file */

	printf("architecture: ");  /* Print architecture label */
	if (Is_BigE(elf_header->e64))
		printf("UNKNOWN!");  /* Print "UNKNOWN!" for big endian format */
	else if (IS_64)
		printf("i386:x86-64");  /* Print "i386:x86-64" for 64-bit format */
	else
		printf("i386");  /* Print "i386" for default format */
	if (EGET(e_type) == ET_REL)
		flags |= HAS_RELOC;  /* Set HAS_RELOC flag if type is relocatable */
	else if (EGET(e_type) == ET_EXEC)
		flags |= EXEC_P;  /* Set EXEC_P flag if type is executable */
	else if (EGET(e_type) == ET_DYN)
		flags |= DYNAMIC;  /* Set DYNAMIC flag if type is dynamic */
	if (EGET(e_phnum) > 0)
		flags |= D_PAGED;  /* Set D_PAGED flag if program header tbl exists */
	if (name_search(elf_header, string_table, ".symtab"))
		flags |= HAS_SYMS;  /* Set HAS_SYMS flag if .symtab section exists */
	if (name_search(elf_header, string_table, ".dynsym"))
		flags |= HAS_SYMS;  /* Set HAS_SYMS flag if .dynsym section exists */
	if (name_search(elf_header, string_table, ".locals"))
		flags |= HAS_LOCALS;  /* Set HAS_LOCALS flag if .locals sec exists */
	if (name_search(elf_header, string_table, ".lineno"))
		flags |= HAS_LINENO;  /* Set HAS_LINENO flag if .lineno sec exists */
	if (name_search(elf_header, string_table, ".debug"))
		flags |= HAS_DEBUG;  /* Set HAS_DEBUG flag if .debug section exists */
	printf(", flags 0x%08lx:\n", flags);  /* Print flags */
	ef_flags(flags);  /* Print flag details */
	printf("start address 0x%0*lx\n\n", IS_64 ? 16 : 8, EGET(e_entry));
	/* Print start address */
}


/*
* name_search - Checks if an ELF file has a given section by name.
* @elf_header: The internal ELF header.
* @string_table: The header string table.
* @section_name: The name of the section to find.
*
* This function searches for a section with the specified name in the ELF file.
*
* Return: 1 if section is found, 0 otherwise.
*/
int name_search(elf_t *elf_header, char *string_table, char *section_name)
{
	size_t i = 0;  /* Loop index */

	section_name = strdup(section_name);  /* Duplicate section name */
	for (i = 1; i < EGET(e_shnum); i++)
	{
		if (!strcmp(string_table + SGET(i, sh_name), section_name))
			return (1);  /* Return true if section found */
	}
	return (0);  /* Return false if section not found */
}

/*
* check_format - Retrieves the string representation of the ELF file format.
* @elf_header: The internal ELF header.
*
* This function returns a string describing the ELF file format.
*
* Return: String representing the ELF file format.
*/
char *check_format(elf_t *elf_header)
{
	static char buf[32];  /* Static buffer for return value */

	*buf = 0;  /* Initialize buffer */
	sprintf(buf, "elf%d-%s", IS_64 ? 64 : 32,
			Is_BigE(elf_header->e64) ? "big" : IS_64 ? "x86-64" : "i386");
	return (buf);  /* Return formatted string */
}
