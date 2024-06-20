
#include "1-hreadelf.h"

void swap_endianess_64(Elf64_Shdr *shdr, int shnum)
{
	int i = 0;

	for (i = 0; i < shnum; i++)
	{
		shdr[i].sh_name = __bswap_32(shdr[i].sh_name);
		shdr[i].sh_type = __bswap_32(shdr[i].sh_type);
		shdr[i].sh_addr = __bswap_64(shdr[i].sh_addr);
		shdr[i].sh_offset = __bswap_64(shdr[i].sh_offset);
		shdr[i].sh_size = __bswap_64(shdr[i].sh_size);
		shdr[i].sh_entsize = __bswap_64(shdr[i].sh_entsize);
		shdr[i].sh_flags = __bswap_64(shdr[i].sh_flags);
		shdr[i].sh_link = __bswap_32(shdr[i].sh_link);
		shdr[i].sh_info = __bswap_32(shdr[i].sh_info);
		shdr[i].sh_addralign = __bswap_64(shdr[i].sh_addralign);
	}
}

void swap_endianess_32(Elf32_Shdr *shdr, int shnum)
{
	int i = 0;

	for (i = 0; i < shnum; i++)
	{
		shdr[i].sh_name = __bswap_32(shdr[i].sh_name);
		shdr[i].sh_type = __bswap_32(shdr[i].sh_type);
		shdr[i].sh_addr = __bswap_32(shdr[i].sh_addr);
		shdr[i].sh_offset = __bswap_32(shdr[i].sh_offset);
		shdr[i].sh_size = __bswap_32(shdr[i].sh_size);
		shdr[i].sh_entsize = __bswap_32(shdr[i].sh_entsize);
		shdr[i].sh_flags = __bswap_32(shdr[i].sh_flags);
		shdr[i].sh_link = __bswap_32(shdr[i].sh_link);
		shdr[i].sh_info = __bswap_32(shdr[i].sh_info);
		shdr[i].sh_addralign = __bswap_32(shdr[i].sh_addralign);
	}
}
