#include "2-hreadelf.h"

/**
 * print_section_to_segment_mapping - self-documenting
 * @EH: the internal header
 * @string_table: the string table
 * Return: 0 on success else error
 */
int print_section_segment_mappings(elf_t *EH, char *string_table)
{
	size_t i, j;

	printf(MAPPING_TITLE);
	for (i = 0; i < EGET(e_phnum); i++)
	{
		printf("   %2.2lu     ", i);
		for (j = 0; j < EGET(e_shnum); j++)
		{
			if (IS_32(EH->e64))
			{
				if (ELF_IS_SECTION_IN_SEGMENT_MEMORY((&EH->s32[j]),
					(&EH->p32[i])))
					printf("%s ", string_table + SGET(j, sh_name));
			}
			else
			{
				if (ELF_IS_SECTION_IN_SEGMENT_MEMORY((&EH->s64[j]),
					(&EH->p64[i])))
					printf("%s ", string_table + SGET(j, sh_name));
			}
		}
		printf("\n");
	}
	return (0);
}
