#include "1-hreadelf.h"

int main(int argc, char **argv)
{
	void *maps;
	size_t filesize;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <elf-file>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	/* 1map_file_into_memory.c */
	maps = map_file_into_memory(argv[1], &filesize);
	if (!maps)
		return (EXIT_FAILURE);

	analyze_prnt_elf_hdrs(maps, filesize); /* 1elfprocess.c */
	munmap(maps, filesize);
	return (EXIT_SUCCESS);
}
