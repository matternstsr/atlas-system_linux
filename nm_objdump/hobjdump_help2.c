#include "hnm.h"

/*
* empty_section - Dumps the contents of a single section in an ELF file.
* @elf_header: Address of the ELF header struct.
* @fd: File descriptor of the ELF file.
* @i: Section index of the current symbol table.
* @string_table: The section header string table.
*
* This function prints the hexadecimal and ASCII representation of the
* contents of a specific section in the ELF file.
*
* Return: Number of symbols printed.
*/
size_t empty_section(elf_t *elf_header, int fd, size_t i, char *string_table)
{
	size_t numps = 0, i_off = 0, j, j_max, addr_len;  /* Vars for iteration */
	unsigned char *data;  /* Data buffer */
	char buf[32] = {0};  /* Buffer for formatting */

	printf("Contents of section %s:\n", string_table + SGET(i, sh_name));
	/* Print section name */
	data = read_data(elf_header, fd, SGET(i, sh_offset), SGET(i, sh_size));
	/* Read section data */
	if (!data)
		return 0;  /* Return if data read fails */
	addr_len = MAX(4, sprintf(buf, "%lx", SGET(i, sh_addr) + SGET(i, sh_size)));
	/* Calculate address length */
	for (i_off = 0; i_off < SGET(i, sh_size); i_off += 0x10)
	{
		j_max = MIN(0x10, SGET(i, sh_size) - i_off);/* calc max bytes print */
		printf(" %0*lx ", (int)addr_len, SGET(i, sh_addr) + i_off);
		/* Print starting address */
		for (j = 0; j < j_max; j++)
			printf("%02x%s", data[i_off + j], !((j + 1) % 4) ? " " : "");
		/* Print hex bytes */
		for (j = j_max; j < 0x10; j++)
			printf("%s%s", "  ", !((j + 1) % 4) ? " " : "");/* Print padding */
		printf(" ");
		for (j = 0; j < j_max; j++)
			printf("%c", (data[i_off + j] >= 32 && data[i_off + j] <= 126) ?
					data[i_off + j] : '.');  /* Print ASCII representation */
		for (j = j_max; j < 0x10; j++)
			printf("%c", ' ');  /* Print ASCII padding */
		printf("\n");
	}
	free(data);  /* Free data buffer */
	return numps;  /* Return number of symbols printed */
	(void)fd;  /* Suppress unused parameter warning */
}

/*
* empty_sections - Dumps the contents of each section in an ELF file.
* @elf_header: Address of the ELF header struct.
* @fd: File descriptor of the ELF file.
* @num_printed: Pointer to variable storing number of symbols printed.
*
* This function iterates through each section of the ELF file, skips certain
* sections based on predefined criteria, and dumps the contents of the rest.
*
* Return: 0 on success, or exit status on failure.
*/
int empty_sections(elf_t *elf_header, int fd, size_t *num_printed)
{
	char *string_table = NULL;  /* Section header string table */
	size_t i;  /* Loop index */

	if (!EGET(e_shnum))
	{
		printf("\nThere are no section headers in this file.\n");
		return 0;
	}

	read_section_headers(elf_header, fd);  /* Read section headers */

	for (i = 0; i < EGET(e_shnum); i++)
		swap_all_endian_section(elf_header, i);  /* Swap endianness each sec */
	string_table = read_string_table(elf_header, fd);  /* Read string table */
	essential_information(elf_header, string_table);  /* Print ELF hdr info*/
	for (i = 1; i < EGET(e_shnum); i++)
	{
		if (strcmp(string_table + SGET(i, sh_name), ".dynstr") &&
			((!strncmp(string_table + SGET(i, sh_name), ".rel", 4) &&
			!SGET(i, sh_addr))
			|| SGET(i, sh_type) == SHT_SYMTAB ||
			SGET(i, sh_type) == SHT_NOBITS ||
			SGET(i, sh_type) == SHT_STRTAB ||
			!SGET(i, sh_size)))
			continue;  /* Skip sections that do not meet the criteria */
		*num_printed += empty_section(elf_header, fd, i, string_table);
		/* Dump section */
	}
	free(string_table);  /* Free string table */
	return 0;  /* Return success */
}
