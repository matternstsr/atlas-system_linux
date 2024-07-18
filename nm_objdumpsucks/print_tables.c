#include "hnm.h"

/**
* print_all_symbol_tables - prints all the symbol table stuff
* @elf_header: address of elf header struct
* @fd: the file descriptor of our ELF file
* @num_printed: pointer to var storing number of symbols printed
* return: 0 on success else exit_status
*/
int print_all_symbol_tables(elf_t *elf_header, int fd, size_t *num_printed)
{
	char *string_table = NULL;   /* Pointer to store string table */
	size_t i;                    /* Loop counter */

	/* If there are no section headers, print message and return 0 */
	if (!EGET(e_shnum))
	{
		printf("\nThere are no section headers in this file.\n");
		return (0);
	}
	/* Read all section headers from ELF file */
	read_section_headers(elf_header, fd);
	/* Swap endianness of all section headers */
	for (i = 0; i < EGET(e_shnum); i++)
		swap_all_endian_section(elf_header, i);
	/* Read string table associated with section headers */
	string_table = read_string_table(elf_header, fd);
	/* Iterate through all section headers */
	for (i = 0; i < EGET(e_shnum); i++)
	{
		/* If the section type is symbol table, print its contents */
		if (SGET(i, sh_type) == SHT_SYMTAB)
		{
			*num_printed += print_symbol_table(elf_header, fd, i, string_table);
			/* Update printed symbol count */
		}
	}
	/* Free dynamically allocated string table memory */
	free(string_table);
	return (0);   /* return 0 indicating success */
}

/**
* print_symbol_table - prints just one symbol table
* @elf_header: address of elf header struct
* @fd: the file descriptor of our ELF file
* @i: section index of current symbol table
* @string_table: the section header string_table
* return: number of symbols printed
*/
size_t print_symbol_table(elf_t *elf_header, int fd, size_t i,
						char *string_table)
{
	char *sym_string_table = NULL;      /* Pointer to symbol string table */
	uint16_t *versym = NULL;            /* Pointer to version symbol table */
	Elf64_Verneed *verneed = NULL;      /* Pointer to version needed table */
	size_t verneed_size = 0, size, j, num_printed;

	/* Calculate number of symbols in the symbol table */
	size = SGET(i, sh_size) / SGET(i, sh_entsize);
	/* Read the symbol table */
	read_symbol_table(elf_header, fd, i);
	/* Swap endianness of all symbols in the symbol table */
	for (j = 0; j < size; j++)
		swap_all_endian_symbol(elf_header, j);
	/* Read symbol string table associated with the symbol table */
	sym_string_table = read_symbol_string_table(elf_header, fd, i + 1);
	/* Determine ELF class (32 or 64-bit) and print symbol table accordingly */
	if (IS_64)
	{
		num_printed = print_symbol_table64(elf_header, string_table,
										sym_string_table, versym,
										verneed, verneed_size, i);
		elf_header->y64 = (free(elf_header->y64), NULL);
		/* Free 64-bit symbol table */
	}
	else
	{
		num_printed = print_symbol_table32(elf_header, string_table,
										sym_string_table, versym,
										verneed, verneed_size, i);
		elf_header->y32 = (free(elf_header->y32), NULL);
		/* Free 32-bit symbol table */
	}
	/* Free alloc mem for symbol str tbl, ver needed tbl, and ver symbol tbl */
	sym_string_table = (free(sym_string_table), NULL);
	verneed = (free(verneed), NULL);
	versym = (free(versym), NULL);
	return (num_printed);   /* return number of symbols printed */
}


/**
* print_symbol_table32 - prints 32 bit program headers
* @elf_header: address of elf header struct
* @string_table: the string table program
* @sym_string_table: the symbol string table
* @versym: the Elf64_Versym section array
* @verneed: the Elf64_Verneed section array
* @verneed_size: the size of the verneed array
* @section: the symbol section to print
* return: number of symbols printed
*/
size_t print_symbol_table32(elf_t *elf_header, char *string_table,
							char *sym_string_table, uint16_t *versym,
							Elf64_Verneed *verneed,
							size_t verneed_size, int section)
{
	size_t i = 0, num_printed = 0;
	size_t size = SGET(section, sh_size) / SGET(section, sh_entsize);

	/* Iterate through each symbol in the symbol table */
	for (i = 0; i < size; i++)
	{
	/* Skip symbols of type STT_SECTION, STT_FILE, or 1st symbol (index 0) */
		if ((YGET(i, st_info) & 0xf) == STT_SECTION ||
			(YGET(i, st_info) & 0xf) == STT_FILE || !i)
			continue;
		/* Print symbol value if not undefined or weak */
		if (get_nm_type32(elf_header->y32[i], elf_header->s32) != 'U' &&
			get_nm_type32(elf_header->y32[i], elf_header->s32) != 'w')
			printf("%8.8lx ", YGET(i, st_value));
		else
			printf("%8s ", "");   /* Print blank if undefined or weak */
		/* Print symbol type and name */
		printf("%c %s\n",
			get_nm_type32(elf_header->y32[i], elf_header->s32),
			/* Symbol type */
			sym_string_table + YGET(i, st_name));/* Symbol name */
		num_printed++;   /* Increment number of symbols printed */
	}
	return (num_printed);   /* return number of symbols printed */
	/* void lines are unused code; these are not used in this function */
	(void)string_table;
	(void)versym;
	(void)verneed;
	(void)verneed_size;
}

/**
* print_symbol_table64 - prints 64 bit program headers
* @elf_header: address of elf header struct
* @string_table: the string table program (unused)
* @sym_string_table: the symbol string table
* @versym: the Elf64_Versym section array (unused)
* @verneed: the Elf64_Verneed section array (unused)
* @verneed_size: the size of the verneed array (unused)
* @section: the symbol section to print
* return: number of symbols printed
*/
size_t print_symbol_table64(elf_t *elf_header, char *string_table,
							char *sym_string_table, uint16_t *versym,
							Elf64_Verneed *verneed,
							size_t verneed_size, int section)
{
	size_t i = 0, num_printed = 0;
	size_t size = SGET(section, sh_size) / SGET(section, sh_entsize);

	/* Iterate through each symbol in the symbol table */
	for (i = 0; i < size; i++)
	{
	/* Skip symbols of type STT_SECTION, STT_FILE, or 1st symbol (index 0) */
		if ((YGET(i, st_info) & 0xf) == STT_SECTION ||
			(YGET(i, st_info) & 0xf) == STT_FILE || !i)
			continue;
		/* Print symbol value if not undefined or weak */
		if (get_nm_type64(elf_header->y64[i], elf_header->s64) != 'U' &&
			get_nm_type64(elf_header->y64[i], elf_header->s64) != 'w')
			printf("%16.16lx ", YGET(i, st_value));
		else
			printf("%16s ", "");   /* Print blank if undefined or weak */
		/* Print symbol type and name */
		printf("%c %s\n",
			get_nm_type64(elf_header->y64[i], elf_header->s64),
			/* Symbol type */
			sym_string_table + YGET(i, st_name));      /* Symbol name */
		num_printed++;   /* Increment number of symbols printed */
	}
	return (num_printed);   /* return number of symbols printed */
	/* void lines are unused code; these are not used in this function */
	(void)string_table;
	(void)versym;
	(void)verneed;
	(void)verneed_size;
}

/**
* print_verneed_table - prints the verneed table information
* @elf_header: address of elf header struct
* @fd: the file descriptor of our ELF file
* @i: section index of the current symbol table
* @sym_string_table: the symbol string table
* @versym: the Elf64_Versym section array
* @verneed: the Elf64_Verneed section array
*/
void print_verneed_table(elf_t *elf_header, int fd, int i,
						char *sym_string_table, uint16_t *versym, Elf64_Verneed *verneed)
{
	size_t j;

	/* Print debug information */
	printf("i:[%d] size[%lu] entsize[%lu] struct[%lu]\n",
		i, SGET(i, sh_size), SGET(i, sh_entsize), sizeof(Elf64_Verneed));
	/* Print versym array */
	for (j = 0; j < SGET(i - 1, sh_size) / 2; j++)
	{
		printf("%lu:[%hd]\n", j, versym[j]);
	}
	/* Print header for verneed table */
	printf("VERSION COUNT FILEOFFSET  AUXOFFSET NEXTOFFSET\n");
	/* return if verneed array is NULL */
	if (!verneed)
		return;
	/* Print each entry in the verneed table */
	for (j = 0; j < SGET(i, sh_size) / sizeof(Elf64_Verneed); j++)
	{
		printf("%7hu %5hu %10d %10d %10d [%s]\n",
			verneed[j].vn_version,
			verneed[j].vn_cnt,
			verneed[j].vn_file,
			verneed[j].vn_aux,
			verneed[j].vn_next,
			sym_string_table + verneed[j].vn_aux);
	}
	(void)fd;  /* Avoid unused parameter warning */
}
