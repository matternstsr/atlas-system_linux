#include "1-hreadelf.h"

typedef struct {
    uint32_t type;
    const char *name;
} SpecialSectionTypeMapping;

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
     /* Calculate number of elements in special_section_type_mappings */
    size_t num_mappings = sizeof(special_section_type_mappings) / sizeof(special_section_type_mappings[0]);
    size_t i;

    for (i = 0; i < num_mappings; ++i) {
        if (special_section_type_mappings[i].type == sh_type) {
            return special_section_type_mappings[i].name;
        }
    }

    return "UNKNOWN";  /* Default return for unknown types */
}
