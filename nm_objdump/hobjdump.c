#include "hnm.h"

/**
* main - Entry point to process ELF files and display symbol tables.
* @argc: Argument count
* @argv: Argument vector
* return: 0 on success, 1+ on error
*/
int main(int ac, char **argv)
{
    int ret = EXIT_SUCCESS;/* Initialize return status */
    char **_argv = argv;/* Pointer to iterate through arguments */

    if (ac < 2)
        return (fprintf(stderr, USAGE), EXIT_FAILURE);/*Print use on insu args*/
    if (ac == 2)
        return (process_file(argv[1], 0, argv));/*Proc single file only 1 arg*/

    /* Process multiple files */
    while (*++_argv)
        ret += process_file(*_argv, 1, argv);

    return (ret);/* Return cumulative status */
}

/**
* process_file - Process an ELF file and print its symbol tables.
* @file_name: Name of the file to process
* @multiple: Flag indicating if multiple files are being processed
* @argv: Argument vector passed to main
* return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
*/
int process_file(char *file_name, int multiple, char **argv)
{
	elf_t elf_header;
	int fd, exit_status;

	fd = open_and_validate_elf(file_name, &elf_header, argv);
	if (fd == -1)
		return (EXIT_FAILURE); /* return failure if ELF file couldn't be proc */
	if (multiple)
		printf("\n%s:\n", file_name); /* Print filename if proc mult files */
	exit_status = process_and_print_symbols(&elf_header, fd, argv, file_name);
	return (exit_status); /* return exit status */
}

/**
* open_and_validate_elf - Open and validate an ELF file.
* @file_name: Name of the file to process
* @elf_header: Pointer to the elf_t structure to fill with ELF information
* @argv: Argument vector passed to main
* return: File descriptor on success, -1 on failure
*/
int open_and_validate_elf(char *file_name, elf_t *elf_header, char **argv)
{
	int fd;
	size_t r;
	const char *fString = "%s: %s: File format not recognized for 32-bit ELF\n";

	memset(elf_header, 0, sizeof(elf_t)); /* Init elf_header to zero */

	fd = crack_open_file(file_name, 0, argv); /* Open the file */
	if (fd == -1)
		return (-1); /* return failure if file couldn't be opened */
	r = read(fd, &elf_header->e64, sizeof(elf_header->e64)); /* ReadELFheader */
	if (r != sizeof(elf_header->e64) || !is_elf_file((char *)&elf_header->e64))
	{
		fprintf(stderr, "%s: %s: File format not recognized\n",
				argv[0], file_name);
		close(fd);
		return (-1); /* return failure if ELF header is not valid */
	}
	if (IS_32(elf_header->e64))
	{ /* Check if ELF file is 32-bit */
		lseek(fd, 0, SEEK_SET); /* Rewind file pointer */
		r = read(fd, &elf_header->e32, sizeof(elf_header->e32)); /*R32 ELF hdr*/
		if (r != sizeof(elf_header->e32) ||
						!is_elf_file((char *)&elf_header->e32))
		{
			fprintf(stderr, fString, argv[0], file_name);
			close(fd);
			return (-1); /* return failure if 32-bit ELF header is not valid */
		}
	}
	return (fd); /* return file descriptor on success */
}

/**
* process_and_print_symbols - Process an ELF file and print its symbol tables.
* @elf_header: Pointer to the elf_t structure containing ELF information
* @fd: File descriptor of the opened ELF file
* @argv: Argument vector passed to main
* @file_name: Name of the file to process
* return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
*/
int process_and_print_symbols(elf_t *elf_header, int fd, char **argv,
							char *file_name)
{
	int exit_status = EXIT_SUCCESS;
	size_t num_printed = 0;

	swap_all_endian(elf_header); /* Swap endianness of ELF header */

	exit_status = print_all_symbol_tables(elf_header, fd, &num_printed);
	/*Print symbol tables */
	if (exit_status != EXIT_SUCCESS)
		fprintf(stderr, "%s: %s: failed to print symbol tables\n",
				argv[0], file_name);
	else if (num_printed == 0)
		fprintf(stderr, "%s: %s: no symbols\n", argv[0], file_name);

	free(elf_header->s32); /* Free allocated memory */
	free(elf_header->s64);
	free(elf_header->p32);
	free(elf_header->p64);

	close(fd); /* Close the file */
	return (exit_status); /* return exit status */
}

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
    if (!first)
        printf("\n");/* Print newline if flags were printed */
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
        flags |= D_PAGED;  /* Set D_PAGED flag if program header table exists */

    if (name_search(elf_header, string_table, ".symtab"))
        flags |= HAS_SYMS;  /* Set HAS_SYMS flag if .symtab section exists */

    if (name_search(elf_header, string_table, ".dynsym"))
        flags |= HAS_SYMS;  /* Set HAS_SYMS flag if .dynsym section exists */

    if (name_search(elf_header, string_table, ".locals"))
        flags |= HAS_LOCALS;  /* Set HAS_LOCALS flag if .locals section exists */

    if (name_search(elf_header, string_table, ".lineno"))
        flags |= HAS_LINENO;  /* Set HAS_LINENO flag if .lineno section exists */

    if (name_search(elf_header, string_table, ".debug"))
        flags |= HAS_DEBUG;  /* Set HAS_DEBUG flag if .debug section exists */

    printf(", flags 0x%08lx:\n", flags);  /* Print flags */
    ef_flags(flags);  /* Print flag details */
    printf("start address 0x%0*lx\n\n", IS_64 ? 16 : 8, EGET(e_entry));  /* Print start address */
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
            return 1;  /* Return true if section found */
    }

    return 0;  /* Return false if section not found */
}

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
    size_t numps = 0, i_off = 0, j, j_max, addr_len;  /* Variables for iteration */
    unsigned char *data;  /* Data buffer */
    char buf[32] = {0};  /* Buffer for formatting */

    printf("Contents of section %s:\n", string_table + SGET(i, sh_name));  /* Print section name */

    data = read_data(elf_header, fd, SGET(i, sh_offset), SGET(i, sh_size));  /* Read section data */
    if (!data)
        return 0;  /* Return if data read fails */

    addr_len = MAX(4, sprintf(buf, "%lx", SGET(i, sh_addr) + SGET(i, sh_size)));  /* Calculate address length */

    for (i_off = 0; i_off < SGET(i, sh_size); i_off += 0x10)
    {
        j_max = MIN(0x10, SGET(i, sh_size) - i_off);  /* Calculate maximum bytes to print */

        printf(" %0*lx ", (int)addr_len, SGET(i, sh_addr) + i_off);  /* Print starting address */

        for (j = 0; j < j_max; j++)
            printf("%02x%s", data[i_off + j], !((j + 1) % 4) ? " " : "");  /* Print hex bytes */

        for (j = j_max; j < 0x10; j++)
            printf("%s%s", "  ", !((j + 1) % 4) ? " " : "");  /* Print padding */

        printf(" ");
        
        for (j = 0; j < j_max; j++)
            printf("%c", (data[i_off + j] >= 32 && data[i_off + j] <= 126) ? data[i_off + j] : '.');  /* Print ASCII representation */

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

        *num_printed += empty_section(elf_header, fd, i, string_table);  /* Dump section */
    }

    free(string_table);  /* Free string table */
    return 0;  /* Return success */
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
    static char ffbuf[32];  /* Static buffer for return value */

    *ffbuf = 0;  /* Initialize buffer */
    sprintf(ffbuf, "elf%d-%s", IS_64 ? 64 : 32,
            Is_BigE(elf_header->e64) ? "big" : IS_64 ? "x86-64" : "i386");
    return ffbuf;  /* Return formatted string */
}
