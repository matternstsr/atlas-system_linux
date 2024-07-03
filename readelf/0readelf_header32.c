#include "1-hreadelf.h"
#include "0elfplaybook.h"

int open_elf_file(const char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void read_elf_header(int fd, Elf32_Ehdr *ehdr)
{
	if (read(fd, ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)) {
		perror("read");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

bool determine_endianness(const Elf32_Ehdr *ehdr)
{
	return (ehdr->e_ident[EI_DATA] == ELFDATA2MSB);
}

void print_elf_header(const Elf32_Ehdr *ehdr, bool swap_endian)
{
	int i;
	
	printf("ELF Header:\n");
	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; ++i)
		printf("%02x ", ehdr->e_ident[i]);
	printf("\n");
	printf("  Class:                             %s\n",
			(ehdr->e_ident[EI_CLASS] == ELFCLASS32) ? "ELF32" : "ELF32");
	printf("  Data:                              %s\n",
			(ehdr->e_ident[EI_DATA] == ELFDATA2LSB) ?
				"2's complement, little endian" :
				"2's complement, big endian");
	printf("  Version:                           %u (current)\n",
			(unsigned int)ehdr->e_ident[EI_VERSION]);
	print_osabi(ehdr->e_ident[EI_OSABI]);
	printf("  ABI Version:                       %u\n",
			(unsigned int)ehdr->e_ident[EI_ABIVERSION]);
	print_file_type(swap16(ehdr->e_type, swap_endian));
	print_machine_type(swap16(ehdr->e_machine, swap_endian));
	printf("  Version:                           0x%x\n", ehdr->e_version);
	printf("  Entry point address:               0x%lx\n",
			(unsigned long)ehdr->e_entry);
	printf("  Start of program headers:          %lu (bytes into file)\n",
			(unsigned long)ehdr->e_phoff);
	printf("  Start of section headers:          %lu (bytes into file)\n",
			(unsigned long)ehdr->e_shoff);
	printf("  Flags:                             0x%x\n", ehdr->e_flags);
	printf("  Size of this header:               %u (bytes)\n", ehdr->e_ehsize);
	printf("  Size of program headers:           %u (bytes)\n", ehdr->e_phentsize);
	printf("  Number of program headers:         %u\n", ehdr->e_phnum);
	printf("  Size of section headers:           %u (bytes)\n", ehdr->e_shentsize);
	printf("  Number of section headers:         %u\n", ehdr->e_shnum);
	printf("  Section header string table index: %u\n", ehdr->e_shstrndx);
}
