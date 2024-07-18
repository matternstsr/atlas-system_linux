#include "hnm.h"

/**
 * getSecHeadStrTab - Reads ELF file and retrieves section header string table.
 * @state: Pointer to the object dump state structure.
 * Return: 1 on failure, 0 on success
 */
int getSecHeadStrTab(objdump_state *state)
{
    Elf64_Shdr st_shdr;
    char *buff = NULL;

    /* Retrieve section header string table header */
    st_shdr = state->s_headers[state->f_header.e_shstrndx];
    /* Reset file position indicator to the beginning */
    rewind(state->f_stream);
    /* Move file position indicator to section header string table offset */
    if (fseek(state->f_stream, st_shdr.sh_offset, SEEK_SET) == -1)
        return (1);
    /* Allocate memory for the section header string table */
    buff = malloc(sizeof(char) * (st_shdr.sh_size));
    if (!buff)
        return (1);
    /* Read section header string table into buffer */
    if (fread(buff, 1, st_shdr.sh_size, state->f_stream) != st_shdr.sh_size)
        return (1);
    /* Store the section header string table in state */
    state->sh_strtab = buff;
    return (0);
}
