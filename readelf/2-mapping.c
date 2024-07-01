#include "2-hreadelf.h"

/**
 * print_section_segment_mappings - Prints mappings of sections to segments.
 * @elf_header: Pointer to the ELF header structure.
 * @string_table: Pointer to the string table containing section names.
 * Return: Returns 0 on success, otherwise an error code.
 */
int print_section_segment_mappings(elf_t *elf_header, char *string_table)
{
    size_t i, j;

    printf("Mapping of Sections to Segments:\n");
    for (i = 0; i < EGET(e_phnum); i++)
    {
        printf("   %2lu     ", i);
        for (j = 0; j < EGET(e_shnum); j++)
        {
            if (IS_32(elf_header->e64))
            {
                if (ELF_IS_SECTION_IN_SEGMENT_MEMORY(&elf_header->s32[j],
                                                     &elf_header->p32[i]))
                    printf("%s ", string_table + SGET(j, sh_name));
            }
            else
            {
                if (ELF_IS_SECTION_IN_SEGMENT_MEMORY(&elf_header->s64[j],
                                                     &elf_header->p64[i]))
                    printf("%s ", string_table + SGET(j, sh_name));
            }
        }
        printf("\n");
    }
    return 0;
}
