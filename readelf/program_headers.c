#include "2-hreadelf.h"

/**
* read_program_headers_from_file - Reads the program headers from the ELF file.
* @elf_header: Pointer to the internal ELF header structure.
* @fd: File descriptor of the ELF file to read.
*/
void read_program_headers_from_file(elf_t *elf_header, int fd)
{
	size_t num_headers = EGET(e_phnum);
	size_t header_size = EGET(e_phentsize);
	size_t total_size = num_headers * header_size;
	char *headers;

	if (num_headers == 0)
		return;

	headers = calloc(num_headers, header_size);
	if (!headers)
		exit(1);

	lseek(fd, EGET(e_phoff), SEEK_SET);
	if (read(fd, headers, total_size) < total_size)
		exit(1); /* TODO */

	if (IS_32(elf_header->e64))
		elf_header->p32 = (void *)headers;
	else
		elf_header->p64 = (void *)headers;
}

/**
* print_program_headers_32bit - Prints 32-bit program headers.
* @elf_header: Address of the ELF header structure.
* @string_table: Pointer to the string table containing program names.
* @fd: File descriptor of the ELF file.
*/
void print_program_headers_32bit(elf_t *elf_header, char *string_table, int fd)
{
	size_t i = 0;

	printf("Program Headers (32-bit):\n");
	for (i = 0; i < EGET(e_phnum); i++)
	{
		printf("  " FORMAT_PROGRAM_32,
			get_segment_type(PGET(i, p_type)),
			PGET(i, p_offset),
			PGET(i, p_vaddr),
			PGET(i, p_paddr),
			PGET(i, p_filesz),
			PGET(i, p_memsz),
			(PGET(i, p_flags) & PF_R) ? 'R' : ' ',
			(PGET(i, p_flags) & PF_W) ? 'W' : ' ',
			(PGET(i, p_flags) & PF_X) ? 'E' : ' ',
			PGET(i, p_align));

		switch (PGET(i, p_type))
		{
			case PT_INTERP:
			{
				char fmt[32], prog[PATH_MAX];
				FILE *file = fdopen(fd, "r");

				snprintf(fmt, sizeof(fmt), "%%%ds", PATH_MAX);
				lseek(fd, PGET(i, p_offset), SEEK_SET);
				if (fscanf(file, fmt, prog) > 0)
					printf("    [Requesting program interpreter: %s]\n", prog);

				fclose(file);
			}
		}
	}
	(void)string_table;
}

/**
* print_program_headers_64bit - Prints 64-bit program headers.
* @elf_header: Address of the ELF header structure.
* @string_table: Pointer to the string table containing program names.
* @fd: File descriptor of the ELF file.
*/
void print_program_headers_64bit(elf_t *elf_header, char *string_table, int fd)
{
	size_t i = 0;

	printf("Program Headers (64-bit):\n");
	for (i = 0; i < EGET(e_phnum); i++)
	{
		printf("  " FORMAT_PROGRAM_64,
			get_segment_type(PGET(i, p_type)),
			PGET(i, p_offset),
			PGET(i, p_vaddr),
			PGET(i, p_paddr),
			PGET(i, p_filesz),
			PGET(i, p_memsz),
			(PGET(i, p_flags) & PF_R) ? 'R' : ' ',
			(PGET(i, p_flags) & PF_W) ? 'W' : ' ',
			(PGET(i, p_flags) & PF_X) ? 'E' : ' ',
			PGET(i, p_align));

		switch (PGET(i, p_type))
		{
			case PT_INTERP:
			{
				char fmt[32], prog[PATH_MAX];
				FILE *file = fdopen(fd, "r");

				snprintf(fmt, sizeof(fmt), "%%%ds", PATH_MAX);
				lseek(fd, PGET(i, p_offset), SEEK_SET);
				if (fscanf(file, fmt, prog) > 0)
					printf("    [Requesting program interpreter: %s]\n", prog);

				fclose(file);
			}
		}
	}
	(void)string_table;
}
