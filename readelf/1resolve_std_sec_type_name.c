#include "1-hreadelf.h"

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
    { SHT_SUNW_syminfo, "VERDEF" },
    { SHT_SUNW_version, "VERNEED" },
    { SHT_SUNW_versym, "VERSYM" },
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
	return ("UNKNOWN"); /* Default return value if not found */
}
