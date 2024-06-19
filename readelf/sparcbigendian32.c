#include "notelf.h"

void sparcbigendian32(const char *filename)
{
	int fd, i;
	Elf32_Ehdr ehdr;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (read(fd, &ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr))
	{
		perror("read");
		close(fd);
		exit(EXIT_FAILURE);
	}

	close(fd);

	/* Print ELF Header */
	printf("ELF Header:\n");
	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; ++i)
		printf("%02x ", ehdr.e_ident[i]);
	printf("\n");

	printf("  Class:                             ELF32\n");
	printf("  Data:                              2's complement, big endian\n");
	printf("  Version:                           %u (current)\n", (unsigned int)ehdr.e_ident[EI_VERSION]);

	print_osabi(ehdr.e_ident[EI_OSABI]);
	printf("  ABI Version:                       %u\n", (unsigned int)ehdr.e_ident[EI_ABIVERSION]);
	print_file_type(bswap_16(ehdr.e_type));  /* Use bswap_16 for correct interpretation*/
	print_machine_type(bswap_16(ehdr.e_machine));  /* Use bswap_16 for correct interpretation*/

	printf("  Version:                           0x%x\n", bswap_32(ehdr.e_version));  /* Use bswap_32 for correct interpretation*/
	printf("  Entry point address:               0x%lx\n", (unsigned long)bswap_32(ehdr.e_entry));  /* Use bswap_32 for correct interpretation*/
	printf("  Start of program headers:          %u (bytes into file)\n", bswap_32(ehdr.e_phoff));  /* Use bswap_32 for correct interpretation*/
	printf("  Start of section headers:          %u (bytes into file)\n", bswap_32(ehdr.e_shoff));  /* Use bswap_32 for correct interpretation*/
	printf("  Flags:                             0x%x\n", bswap_32(ehdr.e_flags));  /* Use bswap_32 for correct interpretation*/
	printf("  Size of this header:               %u (bytes)\n", bswap_16(ehdr.e_ehsize));  /* Use bswap_16 for correct interpretation*/
	printf("  Size of program headers:           %u (bytes)\n", bswap_16(ehdr.e_phentsize));  /* Use bswap_16 for correct interpretation*/
	printf("  Number of program headers:         %u\n", bswap_16(ehdr.e_phnum));  /* Use bswap_16 for correct interpretation*/
	printf("  Size of section headers:           %u (bytes)\n", bswap_16(ehdr.e_shentsize));  /* Use bswap_16 for correct interpretation*/
	printf("  Number of section headers:         %u\n", bswap_16(ehdr.e_shnum));  /* Use bswap_16 for correct interpretation*/
	printf("  Section header string table index: %u\n", bswap_16(ehdr.e_shstrndx));  /* Use bswap_16 for correct interpretation */
}
