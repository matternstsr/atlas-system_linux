#include "1-hreadelf.h"

void analyze_prnt_elf_hdrs(void *maps, size_t filesize)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)maps;
	int is_big_endian, is_64_bit;
	(void)filesize;

	check_elf_magic(ehdr); /* 1check_elf_magic.c*/
	is_big_endian = (ehdr->e_ident[EI_DATA] == ELFDATA2MSB);
	is_64_bit = (ehdr->e_ident[EI_CLASS] == ELFCLASS64);
	if (is_64_bit)
		proc_prnt_elf64_sec(ehdr, is_big_endian, maps);/* SAME FILE*/
	else
		proc_prnt_elf32_sec((Elf32_Ehdr *)ehdr, is_big_endian, maps);/* SAME FILE*/
}
void proc_prnt_elf32_sec(Elf32_Ehdr *ehdr32, int is_big_endian, void *maps)
{
	Elf32_Shdr *shdr32;
	const char *strtab = NULL;
	uint16_t e_shstrndx = ehdr32->e_shstrndx;

	if (is_big_endian)
	{
		e_shstrndx = __bswap_16(ehdr32->e_shstrndx);
		shdr32 = (Elf32_Shdr *)((uint8_t *)maps + __bswap_32(ehdr32->e_shoff));
		swap_endianess_32(shdr32, __bswap_16(ehdr32->e_shnum));
		strtab = (const char *)((uint8_t *)maps +
			shdr32[e_shstrndx].sh_offset);
		print_32bit_sec_hdrs_be(ehdr32, shdr32, strtab);
	}
	else
	{
		shdr32 = (Elf32_Shdr *)((uint8_t *)maps + ehdr32->e_shoff);
		strtab = (const char *)((uint8_t *)maps +
		shdr32[e_shstrndx].sh_offset);
		print_32bit_sec_hdrs(ehdr32, shdr32, strtab);
	}
}

void proc_prnt_elf64_sec(Elf64_Ehdr *ehdr, int is_big_endian, void *maps)
{
	Elf64_Shdr *shdr = (Elf64_Shdr *)((uint8_t *)maps + ehdr->e_shoff);
	const char *strtab = (const char *)((uint8_t *)maps +
		shdr[ehdr->e_shstrndx].sh_offset);

	if (is_big_endian)
		swap_endianess_64(shdr, ehdr->e_shnum); /*1endianess.c*/

	print_64bit_sec_headers(ehdr, shdr, strtab);  /*1print_sec_hdrs.c*/
}
