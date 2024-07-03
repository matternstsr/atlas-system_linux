#include "1-hreadelf.h"

#include <elf.h>

static const SectionTypeMapping standard_section_type_mappings[] = {
	{ SHT_NULL, "NULL" },
	{ SHT_PROGBITS, "PROGBITS" },
	{ SHT_NOTE, "NOTE" },
	{ SHT_HASH, "HASH" },
	{ SHT_DYNSYM, "DYNSYM" },
	{ SHT_STRTAB, "STRTAB" },
	{ SHT_REL, "REL" },
	{ SHT_DYNAMIC, "DYNAMIC" },
	{ SHT_NOBITS, "NOBITS" },
	{ SHT_SYMTAB, "SYMTAB" },
	{ SHT_LOOS + 0xffffff3, "LOOS+ffffff3" },
	{ SHT_LOOS + 0xffffff1, "LOOS+ffffff1" },
	{ SHT_GNU_versym, "VERSYM" },
	{ SHT_GNU_verdef, "VERDEF" },
	{ SHT_GNU_verneed, "VERNEED" },
	{ SHT_GNU_HASH, "GNU_HASH" },
	{ SHT_INIT_ARRAY, "INIT_ARRAY" },
	{ SHT_FINI_ARRAY, "FINI_ARRAY" },
	{ SHT_RELA, "RELA" },
};

/**
* resolve_std_sec_type_name - Resolve std section type name based on sh_type
* @sh_type: The section type to resolve
*
* This function resolves the standard section type specified by @sh_type
* by searching through the array standard_section_type_mappings. If a match
* is found, it returns the corresponding name; otherwise, it returns "UNKNOWN"
* indicating that the section type is not recognized.
*
* @sh_type: The section type to be resolved.
* Return: Pointer to the name corresponding to @sh_type if found;
*         otherwise, "UNKNOWN".
*/
const char *resolve_std_sec_type_name(uint32_t sh_type)
{
	size_t numps = sizeof(standard_section_type_mappings) /
				sizeof(standard_section_type_mappings[0]), i;

	for (i = 0; i < numps; ++i)
	{
		if (standard_section_type_mappings[i].type == sh_type)
			return (standard_section_type_mappings[i].name);
	}
	return ("VERDEF"); /* Default return value if not found */
}
