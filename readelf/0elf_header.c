#include "0elfplaybook.h"

/* Function to read and print ELF header for 64-bit ELF */
void readelf_header64(const char *filename)
{
	Elf64_Ehdr ehdr;

	// Open file
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	// Read ELF header
	if (read(fd, &ehdr, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr)) {
		perror("read");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);

	// Print ELF header
	printf("ELF Header:\n");
	print_elf_identification(&ehdr);
	print_file_information(&ehdr);
	print_program_headers(&ehdr);
	print_section_headers(&ehdr);
}

/* Helper function to print ELF identification */
void print_elf_identification(const Elf64_Ehdr *ehdr)
{
	printf("  Magic:   ");
	for (int i = 0; i < EI_NIDENT; ++i)
		printf("%02x ", ehdr->e_ident[i]);
	printf("\n");
	printf("  Class:                             %s\n",
		(ehdr->e_ident[EI_CLASS] == ELFCLASS64) ? "ELF64" : "ELF32");
	printf("  Data:                              %s\n",
		(ehdr->e_ident[EI_DATA] == ELFDATA2LSB) ?
			"2's complement, little endian" :
			"2's complement, big endian");
	printf("  Version:                           %u (current)\n",
		(unsigned int)ehdr->e_ident[EI_VERSION]);
	print_osabi(ehdr->e_ident[EI_OSABI]);
	printf("  ABI Version:                       %u\n",
		(unsigned int)ehdr->e_ident[EI_ABIVERSION]);
}

/* Helper function to print file information */
void print_file_information(const Elf64_Ehdr *ehdr)
{
	bool swap_endian = (ehdr->e_ident[EI_DATA] == ELFDATA2MSB);
	printf("  Version:                           0x%x\n", ehdr->e_version);
	printf("  Entry point address:               0x%lx\n",
		(unsigned long)ehdr->e_entry);
	printf("  Start of program headers:          %lu (bytes into file)\n",
		(unsigned long)ehdr->e_phoff);
	printf("  Start of section headers:          %lu (bytes into file)\n",
		(unsigned long)ehdr->e_shoff);
	printf("  Flags:                             0x%x\n", ehdr->e_flags);
	printf("  Size of this header:               %u (bytes)\n", ehdr->e_ehsize);
}

/* Helper function to print program headers */
void print_program_headers(const Elf64_Ehdr *ehdr)
{
	int a = ehdr->e_phentsize;
	printf("  Size of program headers:           %u (bytes)\n", a);
	printf("  Number of program headers:         %u\n", ehdr->e_phnum);
}

/* Helper function to print section headers */
void print_section_headers(const Elf64_Ehdr *ehdr)
{
	int b = ehdr->e_shentsize;
	printf("  Size of section headers:           %u (bytes)\n", b);
	printf("  Number of section headers:         %u\n", ehdr->e_shnum);
	printf("  Section header string table index: %u\n", ehdr->e_shstrndx);
}
