#include "elfplaybook.h"

/* Utility functions for endian swapping */
uint16_t swap16(uint16_t val, bool swap)
{
	return (swap ? ((val << 8) | (val >> 8)) : val);
}

uint32_t swap32(uint32_t val, bool swap)
{
	if (swap)
	{
		return (((val << 24) & 0xFF000000) |
						((val <<  8) & 0x00FF0000) |
						((val >>  8) & 0x0000FF00) |
						((val >> 24) & 0x000000FF));
	}
	return (val);
}

/* Function to read ELF header for Sparc big endian 32-bit ELF */
int read_sparcbigendian32(const char *filename, Elf32_Ehdr *ehdr)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}

	if (read(fd, ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr))
	{
		perror("read");
		close(fd);
		return (-1);
	}

	close(fd);
	return (0);
}

/* Function to print ELF header information */
void print_elf_header(const Elf32_Ehdr *ehdr)
{
	int i;

	/* Print ELF Header */
	printf("ELF Header:\n");
	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; ++i)
		printf("%02x ", ehdr->e_ident[i]);
	printf("\n");
	printf("  Class:                             ELF32\n");
	printf("  Data:                              2's complement, big endian\n");
	printf("  Version:                           %u (current)\n",
				(unsigned int)ehdr->e_ident[EI_VERSION]);
	print_osabi(ehdr->e_ident[EI_OSABI]);
	printf("  ABI Version:                       %u\n",
				(unsigned int)ehdr->e_ident[EI_ABIVERSION]);
	print_file_type(bswap_16(ehdr->e_type));
	print_machine_type(bswap_16(ehdr->e_machine));
	printf("  Version:                           0x%x\n",
				bswap_32(ehdr->e_version));
	printf("  Entry point address:               0x%lx\n",
				(unsigned long)bswap_32(ehdr->e_entry));
	printf("  Start of program headers:          %u (bytes into file)\n",
				bswap_32(ehdr->e_phoff));
	printf("  Start of section headers:          %u (bytes into file)\n",
				bswap_32(ehdr->e_shoff));
	printf("  Flags:                             0x%x\n",
				bswap_32(ehdr->e_flags));
	printf("  Size of this header:               %u (bytes)\n",
				bswap_16(ehdr->e_ehsize));
	printf("  Size of program headers:           %u (bytes)\n",
				bswap_16(ehdr->e_phentsize));
	printf("  Number of program headers:         %u\n", bswap_16(ehdr->e_phnum));
	printf("  Size of section headers:           %u (bytes)\n",
				bswap_16(ehdr->e_shentsize));
	printf("  Number of section headers:         %u\n", bswap_16(ehdr->e_shnum));
	printf("  Section header string table index: %u\n",
				bswap_16(ehdr->e_shstrndx));
}

/* Wrapper function to combine reading and printing ELF header */
void sparcbigendian32(const char *filename)
{
	Elf32_Ehdr ehdr;

	if (read_sparcbigendian32(filename, &ehdr) == -1)
	{
		fprintf(stderr, "Error reading ELF header from %s\n", filename);
		exit(EXIT_FAILURE);
	}

	print_elf_header(&ehdr);
}
