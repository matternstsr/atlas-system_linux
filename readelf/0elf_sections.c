/* elf_sections.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "1-hreadelf.h"

/* Function to map section type to string name */
const char *section_type_to_string(unsigned int type)
{
	/*  Existing implementation as provided in 1-hreadelf.c */
	/*  No need to redefine this function here */
	return (section_type_to_string(type));
}

/* Function to convert flags to string representation */
const char *flags_to_string(unsigned long flags)
{
	/*  Existing implementation as provided in 1-hreadelf.c */
	/*  No need to redefine this function here */
	return (flags_to_string(flags));
}

/* Function to read section headers from an ELF file */
Elf64_Shdr *read_section_headers(FILE *fp, Elf64_Ehdr *ehdr)
{
	Elf64_Shdr *shdr = NULL;
	long shoff;
	size_t shdr_size;

	/* Seek to section header table offset */
	shoff = ehdr->e_shoff;
	fseek(fp, shoff, SEEK_SET);
	shdr_size = ehdr->e_shentsize * ehdr->e_shnum;
	shdr = malloc(shdr_size);
	if (shdr == NULL)
	{
		fprintf(stderr, "Error: Failed to allocate memory for section headers\n");
		return (NULL);
	}

	/* Read section headers */
	fread(shdr, ehdr->e_shentsize, ehdr->e_shnum, fp);

	return (shdr);
}

/* Function to display section headers from an ELF file */
void display_section_headers(Elf64_Ehdr *ehdr,
	Elf64_Shdr *shdr, char *shstrtab)
{
	int i;

	/* Print section headers */
	printf("There are %d section headers, starting at offset 0x%lx:\n\n",
			ehdr->e_shnum, (long)ehdr->e_shoff);
	printf("Section Headers:\n");
	printf("  [Nr] Name");
	printf("              Type");
	printf("            Addr");
	printf("     Off");
	printf("    Size");
	printf("   ES");
	printf(" Flg");
	printf(" Lk");
	printf(" Inf");
	printf(" Al\n");

	for (i = 0; i < ehdr->e_shnum; i++)
	{
		const char *name = &shstrtab[shdr[i].sh_name];

		printf("  [%2d] %-17s %-15s %08lx %06lx %06lx %02lx %3s %2d %3d %2d\n",
			i, name, section_type_to_string(shdr[i].sh_type),
			(unsigned long)shdr[i].sh_addr,
			(unsigned long)shdr[i].sh_offset,
			(unsigned long)shdr[i].sh_size,
			(unsigned long)shdr[i].sh_entsize,
			flags_to_string(shdr[i].sh_flags), (int)shdr[i].sh_link,
			(int)shdr[i].sh_info, (int)shdr[i].sh_addralign);

		if (i == 39)   /* Stop printing after the 40th section header */
			break;
	}

	/* Print Key to Flags */
	print_elf_flags_key();
}
