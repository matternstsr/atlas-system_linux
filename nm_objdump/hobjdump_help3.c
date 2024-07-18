#include "hnm.h"

/**
* getSecHeaders - Determines whether to retrieve 32-bit or 64-bit ELF section
* headers based on the file's configuration.
* @state: Pointer to the object dump state containing file data and error info.
* Return: 1 on failure, 0 on success
*/
int getSecHeaders(objdump_state *state)
{
	if (state->f_header.e_shoff)
	{
		/* Select function based on ELF class (32-bit or 64-bit) */
		if (state->ELF_32bit)
			return (get32bitSecHeaders(state));
		else
			return (get64bitSecHeaders(state));
	}
	return (0);
}

/**
* get64bitSecHeaders - Reads and stores 64-bit ELF section headers in the
* provided object dump state.
* @state: Pointer to the object dump state containing file data and error info.
* Return: 1 on failure, 0 on success
*/
int get64bitSecHeaders(objdump_state *state)
{
	Elf64_Shdr *s_headers = NULL;
	int i;

	s_headers = malloc(sizeof(Elf64_Shdr) * state->f_header.e_shnum);
	if (!s_headers)
		return (1);
	if (fseek(state->f_stream, state->f_header.e_shoff, SEEK_SET) == -1)
		return (1);
	if (fread(s_headers, sizeof(Elf64_Shdr), state->f_header.e_shnum,
		state->f_stream) != state->f_header.e_shnum)
		return (1);
	if (state->big_endian)
	{
		for (i = 0; i < state->f_header.e_shnum; i++)
			bswapElf64_Shdr(s_headers + i);
	}
	/* Store 64-bit section headers in the object dump state */
	state->s_headers = s_headers;
	return (0);
}

/* Dynamically allocate memory and read 32-bit ELF section headers */
/**
* get32bitSecHeaders - Reads and stores 32-bit ELF section headers in the
* provided object dump state.
* @state: Pointer to the object dump state containing file data and error info.
* Return: 1 on failure, 0 on success
*/
int get32bitSecHeaders(objdump_state *state)
{
	Elf32_Shdr *s_headers32 = NULL, *curr32 = NULL;
	Elf64_Shdr *s_headers64 = NULL, *curr64 = NULL;
	int i;

	s_headers32 = malloc(sizeof(Elf32_Shdr) * state->f_header.e_shnum);
	s_headers64 = malloc(sizeof(Elf64_Shdr) * state->f_header.e_shnum);
	if (!s_headers32 || !s_headers64)
		return (1);
	if (fseek(state->f_stream, state->f_header.e_shoff, SEEK_SET) == -1)
		return (1);
	if (fread(s_headers32, sizeof(Elf32_Shdr), state->f_header.e_shnum,
		state->f_stream) != state->f_header.e_shnum)
		return (1);
	if (state->big_endian)
	{
		for (i = 0; i < state->f_header.e_shnum; i++)
			bswapElf32_Shdr(s_headers32 + i);
	}
	/* Convert 32-bit section headers to 64-bit format and store */
	for (i = 0; i < state->f_header.e_shnum; i++)
	{
		curr32 = s_headers32 + i;
		curr64 = s_headers64 + i;
		curr64->sh_name      = (Elf64_Word)curr32->sh_name;
		curr64->sh_type      = (Elf64_Word)curr32->sh_type;
		curr64->sh_flags     = (Elf64_Xword)curr32->sh_flags;
		curr64->sh_addr      = (Elf64_Addr)curr32->sh_addr;
		curr64->sh_offset    = (Elf64_Off)curr32->sh_offset;
		curr64->sh_size      = (Elf64_Xword)curr32->sh_size;
		curr64->sh_link      = (Elf64_Word)curr32->sh_link;
		curr64->sh_info      = (Elf64_Word)curr32->sh_info;
		curr64->sh_addralign = (Elf64_Xword)curr32->sh_addralign;
		curr64->sh_entsize   = (Elf64_Xword)curr32->sh_entsize;
	}
	free(s_headers32);
	state->s_headers = s_headers64;
	return (0);
}

/**
* bswapElf64_Shdr - Byte swaps all little-endian values in an Elf64_Shdr
* structure to their big-endian counterparts.
* @shdr64: Pointer to the Elf64_Shdr structure to byte swap.
*/
void bswapElf64_Shdr(Elf64_Shdr *shdr64)
{
	shdr64->sh_name      = __builtin_bswap32(shdr64->sh_name);
	shdr64->sh_type      = __builtin_bswap32(shdr64->sh_type);
	shdr64->sh_flags     = __builtin_bswap64(shdr64->sh_flags);
	shdr64->sh_addr      = __builtin_bswap64(shdr64->sh_addr);
	shdr64->sh_offset    = __builtin_bswap64(shdr64->sh_offset);
	shdr64->sh_size      = __builtin_bswap64(shdr64->sh_size);
	shdr64->sh_link      = __builtin_bswap32(shdr64->sh_link);
	shdr64->sh_info      = __builtin_bswap32(shdr64->sh_info);
	shdr64->sh_addralign = __builtin_bswap64(shdr64->sh_addralign);
	shdr64->sh_entsize   = __builtin_bswap64(shdr64->sh_entsize);
}

/**
* bswapElf32_Shdr - Byte swaps all little-endian values in an Elf32_Shdr
* structure to their big-endian counterparts.
* @shdr32: Pointer to the Elf32_Shdr structure to byte swap.
*/
void bswapElf32_Shdr(Elf32_Shdr *shdr32)
{
	shdr32->sh_name      = __builtin_bswap32(shdr32->sh_name);
	shdr32->sh_type      = __builtin_bswap32(shdr32->sh_type);
	shdr32->sh_flags     = __builtin_bswap32(shdr32->sh_flags);
	shdr32->sh_addr      = __builtin_bswap32(shdr32->sh_addr);
	shdr32->sh_offset    = __builtin_bswap32(shdr32->sh_offset);
	shdr32->sh_size      = __builtin_bswap32(shdr32->sh_size);
	shdr32->sh_link      = __builtin_bswap32(shdr32->sh_link);
	shdr32->sh_info      = __builtin_bswap32(shdr32->sh_info);
	shdr32->sh_addralign = __builtin_bswap32(shdr32->sh_addralign);
	shdr32->sh_entsize   = __builtin_bswap32(shdr32->sh_entsize);
}
