#include "1-hreadelf.h"

void check_elf_magic(Elf64_Ehdr *ehdr)
{
	if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
		ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
		ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
		ehdr->e_ident[EI_MAG3] != ELFMAG3) {
		fprintf(stderr, "Not a valid ELF file\n");
		exit(EXIT_FAILURE);
	}
}
