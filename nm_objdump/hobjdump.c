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
	size_t numps = 0;

	swap_all_endian(elf_header); /* Swap endianness of ELF header */

	exit_status = print_all_symbol_tables(elf_header, fd, &numps);
	/*Print symbol tables */
	if (exit_status != EXIT_SUCCESS)
		fprintf(stderr, "%s: %s: failed to print symbol tables\n",
				argv[0], file_name);
	else if (numps == 0)
		fprintf(stderr, "%s: %s: no symbols\n", argv[0], file_name);

	free(elf_header->s32); /* Free allocated memory */
	free(elf_header->s64);
	free(elf_header->p32);
	free(elf_header->p64);

	close(fd); /* Close the file */
	return (exit_status); /* return exit status */
}



/*
 * dump_all_sections - objdumps each section
 * @elf_header: address of elf header struct
 * @fd: the file descriptor of our ELF file
 * @num_printed: pointer to var storing number of symbols printed
 *
 * This function dumps the contents of each section of an ELF file.
 *
 * Return: 0 on success else exit_status
 */
int dump_all_sections(elf_t *elf_header, int fd, size_t *num_printed)
{
    char *string_table = NULL;/* Section header string table */
    size_t i;/* Loop index */

    if (!EGET(e_shnum))
    {
        printf("\nThere are no section headers in this file.\n");
        return (0);
    }
    read_section_headers(elf_header, fd);/* Read section headers */
    for (i = 0; i < EGET(e_shnum); i++)
        switch_all_endian_section(elf_header, i);/* Switch endianness each sec*/
    string_table = read_string_table(elf_header, fd);/* Read string table */
    print_f_header(elf_header, string_table);/* Print header */
    for (i = 1; i < EGET(e_shnum); i++)
    {
        if (strcmp(string_table + SGET(i, sh_name), ".dynstr") &&
		/* Check if section name is not ".dynstr" */
            ((!strncmp(string_table + SGET(i, sh_name), ".rel", 4) &&
			/* Check if section name starts with ".rel" and sh_addr is 0 */
            !SGET(i, sh_addr))
            || SGET(i, sh_type) == SHT_SYMTAB || /*Chck sec type is symbol tbl*/
            SGET(i, sh_type) == SHT_NOBITS /*Chck sec type is unititial data */
            || SGET(i, sh_type) == SHT_STRTAB  /*Chck sec type is str tbl */
            || !SGET(i, sh_size)))    /* Chck sec is size is zero */
            continue; /* Skip this iteration of the loop */
        *num_printed += dump_section(elf_header, fd, i, string_table);/* Dump section */
    }
    free(string_table);/* Free string table */
    return (0);/* Return success */
}

/*
 * dump_section - dumps just one section
 * @elf_header: address of elf header struct
 * @fd: the file descriptor of our ELF file
 * @i: section index of current symbol table
 * @string_table: the section header string_table
 *
 * This function dumps the contents of a single section of an ELF file.
 *
 * Return: number of symbols printed
 */
size_t dump_section(elf_t *elf_header, int fd, size_t i,
                    char *string_table)
{
    size_t numps = 0, i_off = 0, j, j_max, addr_len;/*vars for iteration*/
    unsigned char *data;/* Data buffer */
    char buf[32] = {0};/* Buffer for formatting */

    printf("Contents of section %s:\n", string_table + SGET(i, sh_name));
	/* Print section name */
    data = read_data(elf_header, fd, SGET(i, sh_offset), SGET(i, sh_size));
	/* Read section data */
    if (!data)
        return (0);/* Return on data read failure */
    addr_len = MAX(4, sprintf(buf, "%lx", SGET(i, sh_addr) + SGET(i, sh_size)));
	/* Calculate address length */
    for (i_off = 0; i_off < SGET(i, sh_size); i_off += 0x10)
    {
        j_max = MIN(0x10, SGET(i, sh_size) - i_off);
		/* Calculate maximum bytes to print */
        printf(" %0*lx ", (int)addr_len, SGET(i, sh_addr) + i_off);
		/* Print starting address */

        for (j = 0; j < j_max; j++)
            printf("%02x%s", data[i_off + j], !((j + 1) % 4) ? " " : "");
		/* Print hex bytes */
        for (j = j_max; j < 0x10; j++)
            printf("%s%s", "  ", !((j + 1) % 4) ? " " : "");/* Print padding */
        printf(" ");
        for (j = 0; j < j_max; j++)
            printf("%c", (data[i_off + j] >= 32
                         && data[i_off + j] <= 126) ? data[i_off + j] : '.');
						 /* Print ASCII representation */
        for (j = j_max; j < 0x10; j++)
            printf("%c", ' ');/* Print ASCII padding */
        printf("\n");
    }
    free(data);/* Free data buffer */
    return (numps);/* Return number of symbols printed */
    (void)fd;/* Suppress unused parameter warning */
}

/*
 * get_file_format - gets the string format type
 * @elf_header: the internal header
 *
 * This function retrieves the string representation of the ELF file format.
 *
 * Return: string type
 */
char *get_file_format(elf_t *elf_header)
{
    static char ffbuf[32];/* Static buffer for return value */

    *ffbuf = 0;/* Initialize buffer */
    sprintf(ffbuf, "elf%d-%s", IS_64 ? 64 : 32,/* Format string */
            IS_BE(elf_header->e64) ? "big" : IS_64 ? "x86-64" : "i386");
    return (ffbuf);/* Return formatted string */
}

/*
 * print_f_header - prints the -f style header
 * @elf_header: the internal header
 * @string_table: the header str table
 *
 * This function prints the -f style header for the ELF file.
 */
void print_f_header(elf_t *elf_header, char *string_table)
{
    unsigned long flags = 0;/* Flags for ELF file */

    printf("architecture: ");/* Print the architecture label */

    if (IS_BE(elf_header->e64))
        printf("UNKNOWN!");/* Print "UNKNOWN!" if IS_BE(e64) */
    else if (IS_64)
        printf("i386:x86-64");/* Print "i386:x86-64" if IS_64 */
    else
        printf("i386");/* Print "i386" by default */
    if (EGET(e_type) == ET_REL)
        flags |= HAS_RELOC;/* Set HAS_RELOC flag if e_type is ET_REL */
    else if (EGET(e_type) == ET_EXEC)
        flags |= EXEC_P;/* Set EXEC_P flag if e_type is ET_EXEC */
    else if (EGET(e_type) == ET_DYN)
        flags |= DYNAMIC;/* Set DYNAMIC flag if e_type is ET_DYN */
    if (EGET(e_phnum) > 0)
        flags |= D_PAGED;/* Set D_PAGED flag if e_phnum is greater than 0 */
    if (has_section(elf_header, string_table, ".symtab"))
        flags |= HAS_SYMS;/* Set HAS_SYMS flag if .symtab section exists */
    if (has_section(elf_header, string_table, ".dynsym"))
        flags |= HAS_SYMS;/* Set HAS_SYMS flag if .dynsym section exists */
    if (has_section(elf_header, string_table, ".locals"))
        flags |= HAS_LOCALS;/* Set HAS_LOCALS flag if .locals section exists */
    if (has_section(elf_header, string_table, ".lineno"))
        flags |= HAS_LINENO;/* Set HAS_LINENO flag if .lineno section exists */
    if (has_section(elf_header, string_table, ".debug"))
        flags |= HAS_DEBUG;/* Set HAS_DEBUG flag if .debug section exists */
    printf(", flags 0x%08lx:\n", flags);/* Print flags */
    print_f_flags(flags);/* Print flag details */
    printf("start address 0x%0*lx\n\n", IS_64 ? 16 : 8, EGET(e_entry));
	/* Print start address */
}

/*
 * has_section - checks is objfile has given section name
 * @elf_header: the internal header
 * @string_table: the header str table
 * @section_name: the name to find
 *
 * This function checks if an ELF file has a given section by name.
 *
 * Return: 1 if found else 0
 */
int has_section(elf_t *elf_header, char *string_table, char *section_name)
{
    size_t i = 0;/* Loop index */

    section_name = strdup(section_name);/* Duplicate section name */
    for (i = 1; i < EGET(e_shnum); i++)
        if (!strcmp(string_table + SGET(i, sh_name), section_name))
            return (1);/* Return true if section found */
    return (0);/* Return false if section not found */
}

/*
 * print_f_flags - prints the string flag list
 * @flags: the flags for this file
 *
 * This function prints the list of flags for the ELF file.
 */
void print_f_flags(unsigned long flags)
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