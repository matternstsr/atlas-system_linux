#include "1-hreadelf.h"
#include "0elfplaybook.h"

/**
* readelf_header64 - Reads and prints ELF header for 64-bit ELF.
* @filename: Name of the file containing the ELF header.
*/
void readelf_header64(const char *filename)
{
	Elf64_Ehdr ehdr;
	bool swap_endian = false;

	if (!read_elf_header64(filename, &ehdr, &swap_endian)) {
		fprintf(stderr, "Failed to read ELF header from file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	print_elf_header64(&ehdr, swap_endian);
}

/**
* read_elf_header64 - Reads the ELF header from a file.
* @filename: Name of the file containing the ELF header.
* @ehdr: Pointer to store the read ELF header structure.
* @swap_endian: Pointer to a boolean indicating if endian swapping is required.
* @return: True if read successfully, false otherwise.
*/
bool read_elf_header64(const char *filename, Elf64_Ehdr *ehdr, bool *swap_endian)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("open");
		return false;
	}

	if (read(fd, ehdr, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr)) {
		perror("read");
		close(fd);
		return false;
	}

	close(fd);

	/* Determine endianness */
	*swap_endian = (ehdr->e_ident[EI_DATA] == ELFDATA2MSB);

	return true;
}

/**
* print_elf_header64 - Prints the contents of the ELF header.
* @ehdr: Pointer to the ELF header structure.
* @swap_endian: Boolean indicating if endian swapping was detected.
*/
void print_elf_header64(const Elf64_Ehdr *ehdr, bool swap_endian)
{
	int a = ehdr->e_phentsize;
	int b = ehdr->e_shentsize;

	/* Print ELF Header */
	printf("ELF Header:\n");
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
	printf("  Size of program headers:           %u (bytes)\n", a);
	printf("  Number of program headers:         %u\n", ehdr->e_phnum);
	printf("  Size of section headers:           %u (bytes)\n", b);
	printf("  Number of section headers:         %u\n", ehdr->e_shnum);
	printf("  Section header string table index: %u\n", ehdr->e_shstrndx);
}
