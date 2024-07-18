#include "hnm.h"

/* Allocate memory for section contents and read into buffer */
/**
 * getSecBuff - read ELF section into buffer as raw bytes
 * @state: struct containing file data and info for error printing
 * @section: pointer to section header to read into buffer
 * Return: NULL on failure, buffer on success
 */
unsigned char *getSecBuff(objdump_state *state, Elf64_Shdr *section)
{
    unsigned char *buff = NULL;

    if (!state || !section)
        return (NULL);

    buff = malloc(sizeof(unsigned char) * section->sh_size);
    if (!buff)
        return (NULL);

    if (fseek(state->f_stream, section->sh_offset, SEEK_SET) == -1)
        return (NULL);

    if (fread(buff, sizeof(unsigned char), section->sh_size,
              state->f_stream) != section->sh_size)
        return (NULL);

    return (buff);
}

/**
 * printSecBuff - print formatted contents of ELF section buffer
 *
 * @buff: buffer containing ELF section contents as raw bytes
 * @section: pointer to section header containing section information
 */
void printSecBuff(unsigned char *buff, Elf64_Shdr *section)
{
    int addr_width;
    uint32_t row;
    char addr_fmt[20];   /* Increase size to allow maximum formatted string */

    if (!buff || !section || section->sh_size == 0)
        return;

    /* Measure printed length of last address in section */
    addr_width = snprintf(NULL, 0, "%lx", section->sh_addr + section->sh_size);
    if (addr_width < 0) {
        /* Handle error (though not likely in practical cases) */
        return;
    }

    /* Generate the format string with appropriate width */
    snprintf(addr_fmt, sizeof(addr_fmt), " %%0%ilx", addr_width > 4 ?
                addr_width : 4);

    for (row = 0; row < section->sh_size / 16; row++)
    {
        /* Print row address */
        printf(addr_fmt, section->sh_addr + (row * 16));
        /* Print buffer in lines of 16 bytes */
        printSecBuffRow(buff, row, 16);
    }

    if (section->sh_size % 16)
    {
        /* Print row address */
        printf(addr_fmt, section->sh_addr + (row * 16));
        /* Print remainder of buffer in last row */
        printSecBuffRow(buff, row, section->sh_size % 16);
    }
}

/**
 * printSecBuffRow - print formatted row of ELF section buffer
 *
 * @buff: buffer containing ELF section contents as raw bytes
 * @row: current row of formatted printing in calling function
 * @bytes: amount of bytes to print in this row
 */
void printSecBuffRow(unsigned char *buff, uint32_t row, uint32_t bytes)
{
    unsigned int col;

    if (!buff || bytes == 0)
        return;

    /* Print hexadecimal representation */
    for (col = 0; col < 16; col++)
    {
        if (col % 4 == 0)
            putchar(' ');
        if (col >= bytes)
        {
            putchar(' ');
            putchar(' ');
        }
        else
            printf("%02x", buff[(row * 16) + col]);
    }
    putchar(' ');
    putchar(' ');
    /* Print ASCII representation */
    for (col = 0; col < 16; col++)
    {
        if (col >= bytes)
            putchar(' ');
        else if (buff[(row * 16) + col] < ' ' ||
                 buff[(row * 16) + col] > '~')
            putchar('.');
        else
            putchar(buff[(row * 16) + col]);
    }
    putchar('\n');
}

/**
 * printSections - print contents of all ELF sections in a file
 * @state: struct containing file data and info for error printing
 * Return: 0 on success, 1 on failure
 */
int printSections(objdump_state *state)
{
    Elf64_Shdr *section = NULL;
    bool bss_reached = false;
    int i;
    unsigned char *sec_buf = NULL;

    if (!state || !state->s_headers)
        return (1);
    for (i = 1; i < state->f_header.e_shnum; i++)
    {
        section = state->s_headers + i;
        if (section->sh_type == SHT_NOBITS &&
            strncmp(state->sh_strtab + section->sh_name, ".bss",
                    5) == 0)
        {
            bss_reached = true;
            continue;
        }
        if (section->sh_size == 0)
            continue;
        if (state->f_header.e_type == ET_REL &&
            (section->sh_type == SHT_RELA ||
             section->sh_type == SHT_REL))
            continue;
        if (bss_reached &&
            (section->sh_type == SHT_SYMTAB ||
             section->sh_type == SHT_STRTAB))
            continue;
        sec_buf = getSecBuff(state, section);
        if (!sec_buf)
            return (1);
        printf("Contents of section %s:\n",
               state->sh_strtab + section->sh_name);
        printSecBuff(sec_buf, section);
        free(sec_buf);
    }
    return (0);
}
