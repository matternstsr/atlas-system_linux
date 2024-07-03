#include "1-hreadelf.h"

static const SpecialSectionTypeMapping special_section_type_mappings[] = {
	{ SHT_GNU_verdef, "VERDEF" },
	{ SHT_GNU_verneed, "VERNEED" },
	{ SHT_GNU_versym, "VERSYM" },
	{ SHT_GNU_HASH, "GNU_HASH" },
	{ SHT_INIT_ARRAY, "INIT_ARRAY" },
	{ SHT_FINI_ARRAY, "FINI_ARRAY" },
	{ SHT_RELA, "RELA" },
};

const char *map_spec_sec_type_name(uint32_t sh_type)
{
    switch (sh_type)
    {
        case SHT_GNU_verdef:
            return "VERDEF";
        case SHT_GNU_verneed:
            return "VERNEED";
        case SHT_GNU_versym:
            return "VERSYM";
        case SHT_GNU_HASH:
            return "GNU_HASH";
        case SHT_INIT_ARRAY:
            return "INIT_ARRAY";
        case SHT_FINI_ARRAY:
            return "FINI_ARRAY";
        case SHT_RELA:
            return "RELA";
        default:
            return "UNKNOWN";
    }
}

case SHT_GNU_verdef:        return "VERDEF";
		case SHT_GNU_verneed:       return "VERNEED";