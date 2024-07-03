#include "2-hreadelf.h"

/**
 * print_program_headers32 - prints 32 bit program headers
 * @EH: address of elf header struct
 * @string_table: the string table program
 * @fd: file descriptor of ELF file
 */
void print_program_headers_32bit(elf_t *EH, char *string_table, int fd)
{
	size_t i = 0;

	printf(TITLE_PROGRAM_32);
	for (i = 0; i < EGET(e_phnum); i++)
	{
		printf(FORMAT_PROGRAM_32,
			get_segment_type(PGET(i, p_type)),
			PGET(i, p_offset),
			PGET(i, p_vaddr),
			PGET(i, p_paddr),
			PGET(i, p_filesz),
			PGET(i, p_memsz),
			PGET(i, p_flags) & PF_R ? 'R' : ' ',
			PGET(i, p_flags) & PF_W ? 'W' : ' ',
			PGET(i, p_flags) & PF_X ? 'E' : ' ',
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
					printf("      [Requesting program interpreter: %s]\n", prog);
				fclose(file);
			}
		}
	}
	(void)string_table;
}

/**
 * print_program_headers64 - prints 64 bit program headers
 * @EH: address of elf header struct
 * @string_table: the string table program
 * @fd: file descriptor of ELF file
 */
void print_program_headers_64bit(elf_t *EH, char *string_table, int fd)
{
	size_t i = 0;

	printf(TITLE_PROGRAM_64);
	for (i = 0; i < EGET(e_phnum); i++)
	{
		printf(FORMAT_PROGRAM_64,
			get_segment_type(PGET(i, p_type)),
			PGET(i, p_offset),
			PGET(i, p_vaddr),
			PGET(i, p_paddr),
			PGET(i, p_filesz),
			PGET(i, p_memsz),
			PGET(i, p_flags) & PF_R ? 'R' : ' ',
			PGET(i, p_flags) & PF_W ? 'W' : ' ',
			PGET(i, p_flags) & PF_X ? 'E' : ' ',
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
					printf("      [Requesting program interpreter: %s]\n", prog);
				fclose(file);
			}
		}
	}
	(void)string_table;
}

/**
 * read_program_headers - reads the program headers into data
 * @EH: the internal header
 * @fd: file descriptor to read
 */
void read_program_headers_from_file(elf_t *EH, int fd)
{
	size_t i = EGET(e_phnum), r;
	char *headers;
	size_t read_size = EGET(e_phentsize) * EGET(e_phnum);

	if (!i)
		return;
	headers = calloc(i, read_size * i);
	if (!headers)
		exit(1);
	lseek(fd, EGET(e_phoff), SEEK_SET);
	r = read(fd, headers, read_size);
	if (r < read_size)
		exit(1); /* TODO */
	if (IS_32(EH->e64))
		EH->p32 = (void *)headers;
	else
		EH->p64 = (void *)headers;
}

