#include "2-hreadelf.h"

/**
* @brief Reads ELF header from file descriptor and validates it.
*
* @param fd File descriptor of the ELF file.
* @param elf_header Pointer to the elf_t structure to populate.
* @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
*/
int read_and_validate_elf_header(int fd, elf_t *elf_header)
{
	size_t r;

	memset(elf_header, 0, sizeof(elf_t));
	r = read(fd, &elf_header->e64, sizeof(elf_header->e64));
	if (r != sizeof(elf_header->e64) ||
					check_elf_header((char *)&elf_header->e64))
	{
		fprintf(stderr, ERR_NOT_MAGIC);
		return (EXIT_FAILURE);
	}
	if (IS_32(elf_header->e64))
	{
		lseek(fd, 0, SEEK_SET);
		r = read(fd, &elf_header->e32, sizeof(elf_header->e32));
		if (r != sizeof(elf_header->e32) ||
						check_elf_header((char *)&elf_header->e32))
		{
			fprintf(stderr, ERR_NOT_MAGIC);
			return (EXIT_FAILURE);
		}
	}
	switch_all_endian(elf_header);
	return (EXIT_SUCCESS);
}

/**
* @brief Displays all ELF program headers.
*
* @param elf_header Pointer to the elf_t structure containing ELF headers.
* @param fd File descriptor of the ELF file.
* @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
*/
int display_all_elf_program_headers(elf_t *elf_header, int fd)
{
	int exit_status = EXIT_SUCCESS;

	if (IS_32(elf_header->e64)) {
		print_program_headers_32bit(elf_header, NULL, fd);
	} else {
		print_program_headers_64bit(elf_header, NULL, fd);
	}

	free(elf_header->s32);
	free(elf_header->s64);
	free(elf_header->p32);
	free(elf_header->p64);

	return exit_status;
}


/**
* @brief Main entry point.
*
* @param ac Argument count.
* @param argv Argument vector.
* @return int 0 on success, 1+ on error.
*/
int main(int ac, char **argv)
{
	int fd, exit_status = EXIT_SUCCESS;
	elf_t elf_header;

	if (ac != 2)
	{
		fprintf(stderr, USAGE);
		return (EXIT_FAILURE);
	}
	fd = open_and_handle_file(argv[1], 0);
	if (fd == -1)
		return (EXIT_FAILURE);
	exit_status = read_and_validate_elf_header(fd, &elf_header);
	if (exit_status == EXIT_SUCCESS)
		exit_status = display_all_elf_program_headers(&elf_header, fd);
	close(fd);
	return (exit_status);
}

/* static char *get_unknown_section_type_name(unsigned int sh_type)
{
	switch (sh_type) {
		case MY_UNKNOWN_SECTION_TYPE:
			return "Unknown";
		default:
			return "Unsupported";
	}
}
 */