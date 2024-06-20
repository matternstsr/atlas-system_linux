#include "1-hreadelf.h"

static const SectionTypeMapping additional_section_type_mappings[] = {
	{ SHT_GNU_verdef, "VERDEF" },
	{ SHT_GNU_verneed, "VERNEED" },
	{ SHT_GNU_versym, "VERSYM" },
	{ SHT_GNU_HASH, "GNU_HASH" },
	{ SHT_INIT_ARRAY, "INIT_ARRAY" },
	{ SHT_FINI_ARRAY, "FINI_ARRAY" },
	{ SHT_RELA, "RELA" },
};

const char *res_spec_sec_type_name(uint32_t sh_type)
{
	/* Calculate number of elements in additional_section_type_mappings */
	size_t num_mappings = sizeof(additional_section_type_mappings) /
												sizeof(additional_section_type_mappings[0]);
	size_t i;

	for (i = 0; i < num_mappings; ++i)
	{
		if (additional_section_type_mappings[i].type == sh_type)
			return (additional_section_type_mappings[i].name);
	}
	return ("UNKNOWN");  /* Default return for unknown types */
}
