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
		{ SHT_SUNW_syminfo, "VERDEF" }, /*.SUNW_syminfo of type SHT_SUNW_syminfo.*/
};

/**
 * resolve_std_sec_type_name - Resolve std section type name based on sh_type
 * @sh_type: The section type to resolve
 *
 * Returns: ptr to name corresp to sh_type if found; ow, "UNKNOWN".
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
