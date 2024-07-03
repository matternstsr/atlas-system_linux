#include "1-hreadelf.h"

static const SectionTypeMapping section_type_mappings[] = {
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
	#ifdef SHT_SUNW_version
	{ SHT_SUNW_version, "VERNEED" },
	#endif
	#ifdef SHT_SUNW_versym
	{ SHT_SUNW_versym, "VERSYM" },
	#endif
	{ SHT_SUNW_syminfo, "VERDEF" },
};


/**
* resolve_section_type - Resolve section type name based on sh_type
* @sh_type: The section type to resolve
*
* This function resolves the section type specified by @sh_type
* by searching through the array section_type_mappings. If a match
* is found, it returns the corresponding name; otherwise, it falls
* back to resolve_section_type_alternative to handle the resolution.
*
* Return: Pointer to the name corresponding to @sh_type if found;
*         otherwise, the result of resolve_section_type_alternative.
*/
const char *resolve_section_type(uint32_t sh_type)
{
	/* Calculate number of elements in section_type_mappings */
	size_t numps = sizeof(section_type_mappings) /
								sizeof(section_type_mappings[0]);
	size_t i;

	for (i = 0; i < numps; ++i)
	{
		if (section_type_mappings[i].type == sh_type)
			return (section_type_mappings[i].name);
	}
	return (resolve_section_type_alternative(sh_type));
}
