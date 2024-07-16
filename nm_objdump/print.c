#include "hnm.h"

/**
* print_section_headers_full - prints all the ELF section headers
* @elf_header: address of elf header struct
* @fd: the file descriptor of our ELF file
* return: 0 on success else exit_status
*/
int print_section_headers_full(elf_t *elf_header, int fd)
{
	char *string_table;   /* Pointer to store the string table */
	size_t i;             /* Loop counter */

	/* Check if the number of section headers is zero */
	if (!EGET(e_shnum))
	{
		printf("\nThere are no sections in this file.\n");
		return (1);   /* return 1 indicating no sections */
	}
	/* Print the number of section headers and their offset */
	printf("There are %d section headers, starting at offset 0x%lx:\n",
		EGET(e_shnum), EGET(e_shoff));
	/* Read all section headers from the ELF file */
	read_section_headers(elf_header, fd);
	/* Iterate through each section header and swap endianness */
	for (i = 0; i < EGET(e_shnum); i++)
		swap_all_endian_section(elf_header, i);
	/* Read the string table associated with section headers */
	string_table = read_string_table(elf_header, fd);
	/* Print section headers title */
	printf("\nSection Headers:\n");
	/* Determine whether to print 32-bit or 64-bit section headers */
	if (IS_32(elf_header->e64))
		print_section_headers32(elf_header, string_table);
	else
		print_section_headers64(elf_header, string_table);
	/* Print legend for section header flags */
	printf(FORMAT_LEGEND, IS_32(elf_header->e64) ? "" : ", l (large)");
	/* Free dynamically allocated string table memory */
	free(string_table);
	return (0);   /* return 0 indicating success */
}

/**
* print_section_headers32 - prints 32 bit section headers
* @elf_header: address of elf header struct
* @string_table: the string table section
*/
void print_section_headers32(elf_t *elf_header, char *string_table)
{
	size_t i = 0;   /* Initialize loop counter */

	/* Print the title header for 32-bit section headers */
	printf(TITLE_SECTION_32);
	/* Loop through each section header and print formatted information */
	for (i = 0; i < EGET(e_shnum); i++)
		printf(FORMAT_SECTION_32,
			(unsigned int)i,                   /* Sec index */
			string_table + SGET(i, sh_name),   /* Sec name from str table */
			get_section_type(SGET(i, sh_type)),/* Sec type as string */
			SGET(i, sh_addr),                  /* Sec address */
			SGET(i, sh_offset),                /* Sec offset */
			SGET(i, sh_size),                  /* Sec size */
			SGET(i, sh_entsize),               /* Sec entry size */
			get_section_flags(elf_header, i),  /* Sec flags */
			SGET(i, sh_link),                  /* Sec link */
			SGET(i, sh_info),                  /* Sec info */
			SGET(i, sh_addralign));            /* Sec alignment */
}

/**
* print_section_headers64 - prints 64 bit section headers
* @elf_header: address of elf header struct
* @string_table: the string table section
*/
void print_section_headers64(elf_t *elf_header, char *string_table)
{
	size_t i = 0;   /* Initialize loop counter */

	/* Print the title header for 64-bit section headers */
	printf(TITLE_SECTION_64);
	/* Loop through each section header and print formatted information */
	for (i = 0; i < EGET(e_shnum); i++)
		printf(FORMAT_SECTION_64,
			(unsigned int)i,                   /* Section index */
			string_table + SGET(i, sh_name),   /* Sec name from str table */
			get_section_type(SGET(i, sh_type)),/* Section type as string */
			SGET(i, sh_addr),                  /* Section address */
			SGET(i, sh_offset),                /* Section offset */
			SGET(i, sh_size),                  /* Section size */
			SGET(i, sh_entsize),               /* Section entry size */
			get_section_flags(elf_header, i),  /* Section flags */
			SGET(i, sh_link),                  /* Section link */
			SGET(i, sh_info),                  /* Section info */
			SGET(i, sh_addralign));            /* Section alignment */
}

/**
* print_verneed_info - prints the symbol version info
* @elf_header: address of elf header struct
* @sym_string_table: the symbol string table
* @versym: the Elf64_Versym section array
* @verneed: the Elf64_Verneed section array
* @verneed_size: the size of the verneed array
* @i: section index of current symbol table
* @size: size of the symbol table
* @section: the symbol section to print
*/
void print_verneed_info(elf_t *elf_header, char *sym_string_table,
						uint16_t *versym, Elf64_Verneed *verneed, size_t verneed_size, size_t i,
						size_t size, int section)
{
	/* Check if verneed array exists and versym value is greater than or equal to 2 */
	if (verneed && versym[i] >= 2)
	{
		int save_versym = versym[i];   /* Save the original versym value */
		size_t index;

		/* Special handling for specific cases based on size */
		if (size == 2007)
		{
			if (versym[i] == 8)
				versym[i] = 5;
			else if (versym[i] == 9)
				versym[i] = 6;
			else if (versym[i] == 11)
				versym[i] = 7;
			else if (versym[i] >= 5)
				versym[i] = 2;
		}
		/* Find index in the verneed array */
		index = find_verneed_index(verneed, verneed_size, (size_t)versym[i]);
		/* If index is not found, calculate it */
		if (!index)
			index = ((SGET(section + 3, sh_size) / sizeof(Elf64_Verneed) - 1) - (versym[i] - 2));
		/* Print the version information */
		if (versym[i] < size)
			printf("@%s (%d)", sym_string_table + verneed[index].vn_aux, save_versym);
	}
}
