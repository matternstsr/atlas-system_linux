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
};

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
