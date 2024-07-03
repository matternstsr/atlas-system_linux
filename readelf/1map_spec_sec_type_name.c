#include "1-hreadelf.h"

static const SectionTypeMapping additional_section_type_mappings[] = {
    { SHT_GNU_verdef, "VERDEF" },
    { SHT_GNU_verneed, "VERNEED" },
    { SHT_GNU_versym, "VERSYM" },
    { SHT_GNU_HASH, "GNU_HASH" },
    { SHT_INIT_ARRAY, "INIT_ARRAY" },
    { SHT_FINI_ARRAY, "FINI_ARRAY" },
    { SHT_RELA, "RELA" },
    { SHT_SUNW_version, "SUNW_version" },   // Added .SUNW_version mapping
    { SHT_SUNW_versym, "SUNW_versym" },     // Added .SUNW_versym mapping
};

const char *map_spec_sec_type_name(uint32_t sh_type)
{
		/* Calculate number of elements in special_section_type_mappings */
	size_t numps = sizeof(special_section_type_mappings) /
								sizeof(special_section_type_mappings[0]);
	size_t i;

	for (i = 0; i < numps ; ++i)
	{
		if (special_section_type_mappings[i].type == sh_type)
			return (special_section_type_mappings[i].name);
	}
	return ("UNKNOWN");  /* Default return for unknown types */
}
