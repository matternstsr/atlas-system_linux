#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "elf.h"
#include "endian.h"

/* Function to print OS/ABI */
void print_osabi(unsigned char osabi)
{
		printf("  OS/ABI:                            ");
		switch (osabi)
		{
				case ELFOSABI_SYSV:
						printf("UNIX - System V\n");
						break;
				case ELFOSABI_SOLARIS:
						printf("UNIX - Solaris\n");
						break;
				case ELFOSABI_SORTIX:
						printf("UNIX - Sortix\n");
						break;
				default:
						printf("<unknown: %x>\n", osabi);
						break;
		}
}

/* Function to print file type */
void print_file_type(uint16_t type)
{
		printf("  Type:                              ");
		switch (type)
		{
				case ET_NONE:
						printf("NONE (Unknown file type)\n");
						break;
				case ET_REL:
						printf("REL (Relocatable file)\n");
						break;
				case ET_EXEC:
						printf("EXEC (Executable file)\n");
						break;
				case ET_DYN:
						printf("DYN (Shared object file)\n");
						break;
				case ET_CORE:
						printf("CORE (Core file)\n");
						break;
				default:
						printf("Unknown\n");
						break;
		}
}

/* Function to print machine type */
void print_machine_type(uint16_t machine)
{
		printf("  Machine:                           ");
		switch (machine)
		{
				case EM_X86_64:
						printf("Advanced Micro Devices X86-64\n");
						break;
				case EM_386:
						printf("Intel 80386\n");
						break;
				case EM_SPARC:
						printf("Sparc\n");
						break;
				case EM_SPARC32PLUS:
						printf("Sparc v8+\n");
						break;
				case EM_SPARCV9:
						printf("Sparc v9\n");
						break;
				default:
						printf("Unknown\n");
						break;
		}
}
