#include "hnm.h"     /* Include header file hnm.h */
   /* Define multi-line string for format legend */
#define FORMAT_LEGEND \
	"Key to Flags:\n" \
	"  W (write), A (alloc), X (execute), M (merge), S (strings)%s\n" \
	"  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n" \
	"  O (extra OS processing required) o (OS specific), p (processor specific)\n"
   /* Define title format for 32-bit sections */
#define TITLE_SECTION_32 \
    "  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n"
   /* Define format for 32-bit sections */
#define FORMAT_SECTION_32 \
    "  [%2u] %-17s %-15.15s %8.8lx %6.6lx %6.6lx %2.2lx %3s %2u %3u %2lu\n"
   /* Define title format for 64-bit sections */
#define TITLE_SECTION_64 \
    "  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n"
   /* Define format for 64-bit sections */
#define FORMAT_SECTION_64 \
    "  [%2u] %-17s %-15.15s %16.16lx %6.6lx %6.6lx %2.2lx %3s %2u %3u %2lu\n"
   /* Define format for 64-bit symbols */
#define FORMAT_SYMBOL_64 \
    "%16.16lx %c %s"
   /* Define format for 32-bit symbols */
#define FORMAT_SYMBOL_32 \
    "%8.8lx %c %s"


/**
 * read_section_headers - reads the section headers into data
 * @elf_header: the internal header
 * @fd: file descriptor to read
 */
void read_section_headers(elf_t *elf_header, int fd)
{
    size_t i = EGET(e_shnum), r;   /* Initialize i with the number of section headers */
    char *headers;                  /* Pointer to store section headers */
    size_t read_size = EGET(e_shentsize) * EGET(e_shnum);   /* Calculate total size to read */

    /* If there are no section headers, return */
    if (!i)
        return;

    /* Allocate memory to store section headers */
    headers = calloc(i, read_size);
    if (!headers)
        exit(1);   /* Exit if allocation fails */

    /* Seek to the offset where section headers start */
    lseek(fd, EGET(e_shoff), SEEK_SET);

    /* Read section headers from file */
    r = read(fd, headers, read_size);

    /* If read size doesn't match expected size, exit (TODO: handle better) */
    if (r < read_size)
        exit(1);   /* Exit if read fails */

    /* Assign the read headers to the appropriate structure field based on ELF type */
    if (IS_32(elf_header->e64))
        elf_header->s32 = (void *)headers;
    else
        elf_header->s64 = (void *)headers;
}

/**
 * read_string_table - reads the string table
 * @elf_header: the internal header
 * @fd: file descriptor to read
 * Return: pointer to beginning of table
 */
char *read_string_table(elf_t *elf_header, int fd)
{
    char *str;   /* Pointer to store string table */

    /* Allocate memory for the string table */
    str = calloc(1, SGET(EGET(e_shstrndx), sh_size));
    if (!str)
        exit(1);   /* Exit if allocation fails */

    /* Seek to the offset where string table starts */
    lseek(fd, SGET(EGET(e_shstrndx), sh_offset), SEEK_SET);

    /* Read the string table from file */
    read(fd, str, SGET(EGET(e_shstrndx), sh_size));

    return (str);   /* Return pointer to the beginning of the table */
}

/**
 * read_symbol_table - reads the symbol headers into data
 * @elf_header: the internal header
 * @fd: file descriptor to read
 * @i: the current symbol section to print
 */
void read_symbol_table(elf_t *elf_header, int fd, int i)
{
    size_t r;
    char *headers;
    size_t read_size = SGET(i, sh_size);

    /* Allocate memory for symbol headers */
    headers = calloc(1, read_size);
    if (!headers)
        exit(1);  /* Exit if memory allocation fails */

    /* Seek to the offset of the symbol section and read data */
    lseek(fd, SGET(i, sh_offset), SEEK_SET);
    r = read(fd, headers, read_size);

    /* Check if read operation was successful */
    if (r < read_size)
    {
        fprintf(stderr, "Symbol read failed.\n");
        exit(1);  /* Exit if read operation failed */
    }

    /* Assign the read data to the appropriate member in elf_header based on ELF class */
    if (IS_32(elf_header->e64))
        elf_header->y32 = (void *)headers;
    else
        elf_header->y64 = (void *)headers;
}


/**
 * read_symbol_string_table - reads the symbol string table
 * @elf_header: the internal header
 * @fd: file descriptor to read
 * @i: the current symbol section index
 * Return: pointer to the beginning of the symbol string table
 */
char *read_symbol_string_table(elf_t *elf_header, int fd, int i)
{
    char *str;

    /* Allocate memory for the symbol string table */
    str = calloc(1, SGET(i, sh_size));
    if (!str)
        exit(1);  /* Exit if memory allocation fails */

    /* Seek to the offset of the symbol string table and read data */
    lseek(fd, SGET(i, sh_offset), SEEK_SET);
    read(fd, str, SGET(i, sh_size));

    return str;  /* Return a pointer to the beginning of the symbol string table */
}


size_t find_verneed_index(Elf64_Verneed *verneed, size_t verneed_size,
                          size_t index)
{
    size_t i = 0, ret = 0;

    /* Iterate through the verneed array */
    for (i = 1; i < verneed_size; i++)
    {
        /* If vn_file field is out of bounds, return 0 */
        if (verneed[i].vn_file >= verneed_size)
            return (0);

        /* If vn_file matches the index, update the return value */
        if (verneed[i].vn_file == index)
            ret = i;
    }

    return (ret);  /* Return the index of the verneed entry, or 0 if not found */
}

/**
 * read_data - reads a block of data from descriptor
 * @elf_header: the internal header (unused)
 * @fd: the descriptor to read from
 * @offset: offset to start reading from
 * @size: number of bytes to read
 * Return: pointer to the allocated data block
 */
void *read_data(elf_t *elf_header, int fd, unsigned long offset, long size)
{
    void *str = NULL;

    /* Allocate memory for the data block */
    str = calloc(1, size);
    if (!str)
        exit(1);  /* Exit if allocation fails */

    /* Seek to the specified offset in the file */
    lseek(fd, offset, SEEK_SET);

    /* Read 'size' bytes of data from the file descriptor 'fd' */
    if (read(fd, str, size) != size)
    {
        fprintf(stderr, "Read error.\n");
        exit(1);  /* Exit if read fails */
    }

    return (str);  /* Return the pointer to the allocated data block */
    
    (void)elf_header;  /* Unused parameter (suppress compiler warning) */
}
