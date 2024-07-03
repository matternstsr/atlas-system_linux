#include "1-hreadelf.h"

// Function to resolve section type name
const char *res_spec_sec_type_name(uint32_t sh_type)
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
        case SHT_SUNW_version:
            return "SUNW_version";
        case SHT_SUNW_versym:
            return "SUNW_versym";
        default:
            return "UNKNOWN";
    }
}