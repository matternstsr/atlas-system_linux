#include "2-hreadelf.h"

/**
* @brief Reads ELF header from file descriptor and validates it.
*
* @param fd File descriptor of the ELF file.
* @param EH Pointer to the elf_t structure to populate.
* @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
*/
int read_and_validate_elf_header(int fd, elf_t *EH)
{
	size_t r;

	memset(EH, 0, sizeof(elf_t));
	r = read(fd, &EH->e64, sizeof(EH->e64));
	if (r != sizeof(EH->e64) ||
					check_elf_header((char *)&EH->e64))
	{
		fprintf(stderr, ERR_NOT_MAGIC);
		return (EXIT_FAILURE);
	}

	if (IS_32(EH->e64))
	{
		lseek(fd, 0, SEEK_SET);
		r = read(fd, &EH->e32, sizeof(EH->e32));
		if (r != sizeof(EH->e32) ||
						check_elf_header((char *)&EH->e32))
		{
			fprintf(stderr, ERR_NOT_MAGIC);
			return (EXIT_FAILURE);
		}
	}
	switch_all_endian(EH);
	return (EXIT_SUCCESS);
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
	elf_t EH;

	if (ac != 2)
	{
		fprintf(stderr, USAGE);
		return (EXIT_FAILURE);
	}
	fd = open_and_handle_file(argv[1], 0);
	if (fd == -1)
		return (EXIT_FAILURE);
	exit_status = read_and_validate_elf_header(fd, &EH);
	if (exit_status == EXIT_SUCCESS)
		exit_status = display_all_elf_program_headers(&EH, fd);
	close(fd);
	return (exit_status);
}
