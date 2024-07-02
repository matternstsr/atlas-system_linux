#include "2-hreadelf.h"

/**
* get_segment_type_name - Retrieves the string representation of a segment type.
* @p_type: The segment type identifier (p_type).
* Return: String representation of the segment type.
*/
char *get_segment_type_name(unsigned long p_type)
{
    switch (p_type)
    {
        case PT_NULL:           return "PT_NULL";
        case PT_LOAD:           return "PT_LOAD";
        case PT_DYNAMIC:        return "PT_DYNAMIC";
        case PT_INTERP:         return "PT_INTERP";
        case PT_NOTE:           return "PT_NOTE";
        case PT_SHLIB:          return "PT_SHLIB";
        case PT_PHDR:           return "PT_PHDR";
        case PT_TLS:            return "PT_TLS";
        case PT_GNU_EH_FRAME:   return "PT_GNU_EH_FRAME";
        case PT_GNU_STACK:      return "PT_GNU_STACK";
        case PT_GNU_RELRO:      return "PT_GNU_RELRO";
        case 0x6464E550:        return "LOOS+464e550";
        default:                return "UNKNOWN";
    }
}


/**
* get_elf_file_type - Retrieves the ELF file type as a string.
* @elf_header: Pointer to the ELF header structure.
* Return: String representation of the ELF file type.
*/
char *get_elf_file_type(elf_t *elf_header)
{
    static char type_str[64]; // Buffer for storing the ELF file type string

    // Determine the ELF file type and return its corresponding string representation
    switch (EGET(e_type))
    {
        case ET_NONE:   return "NONE (None)";
        case ET_REL:    return "REL (Relocatable file)";
        case ET_EXEC:   return "EXEC (Executable file)";
        case ET_DYN:    return "DYN (Shared object file)";
        case ET_CORE:   return "CORE (Core file)";
        default:
            snprintf(type_str, sizeof(type_str), "<unknown>: %x", EGET(e_type));
            return type_str;
    }
}


/**
* get_section_type_name - Returns the string name corresponding to a section type.
* @sh_type: The section type identifier.
* Return: String name of the section type.
*/
char *get_section_type_name(unsigned int sh_type)
{
    static char type_name[64]; // Buffer for storing the section type name

    // Determine the section type and return its corresponding string name
    switch (sh_type)
    {
        case SHT_NULL:              return "NULL";
        case SHT_PROGBITS:          return "PROGBITS";
        case SHT_SYMTAB:            return "SYMTAB";
        case SHT_STRTAB:            return "STRTAB";
        case SHT_RELA:              return "RELA";
        case SHT_HASH:              return "HASH";
        case SHT_DYNAMIC:           return "DYNAMIC";
        case SHT_NOTE:              return "NOTE";
        case SHT_NOBITS:            return "NOBITS";
        case SHT_REL:               return "REL";
        case SHT_SHLIB:             return "SHLIB";
        case SHT_DYNSYM:            return "DYNSYM";
        case SHT_INIT_ARRAY:        return "INIT_ARRAY";
        case SHT_FINI_ARRAY:        return "FINI_ARRAY";
        case SHT_PREINIT_ARRAY:     return "PREINIT_ARRAY";
        case SHT_GNU_HASH:          return "GNU_HASH";
        case SHT_GROUP:             return "GROUP";
        case SHT_SYMTAB_SHNDX:      return "SYMTAB SECTION INDICES";
        case SHT_GNU_verdef:        return "VERDEF";
        case SHT_GNU_verneed:       return "VERNEED";
        case SHT_GNU_versym:        return "VERSYM";
        case 0x6ffffff0:            return "VERSYM";
        case 0x6ffffffc:            return "VERDEF";
        case 0x7ffffffd:            return "AUXILIARY";
        case 0x7fffffff:            return "FILTER";
        case 0x6ffffff1:            return "LOOS+ffffff1";
        case 0x6ffffff3:            return "LOOS+ffffff3";
        case SHT_GNU_LIBLIST:       return "GNU_LIBLIST";
        default:
            snprintf(type_name, sizeof(type_name), "%08x: <unknown>", sh_type);
            return type_name;
    }
}


/**
* translate_section_flags - Translates section flags into a string representation.
* @elf_header: Pointer to the ELF header structure.
* @section_index: Index of the section whose flags to translate.
* Return: String representation of section flags based on legend.
*/
char *translate_section_flags(elf_t *elf_header, size_t section_index)
{
    static char flags_string[32];
    unsigned long flags = SGET(section_index, sh_flags);
    char *p = flags_string;

    // Translate each flag into its corresponding character
    if (flags & SHF_WRITE)         *p++ = 'W';
    if (flags & SHF_ALLOC)         *p++ = 'A';
    if (flags & SHF_EXECINSTR)     *p++ = 'X';
    if (flags & SHF_MERGE)         *p++ = 'M';
    if (flags & SHF_STRINGS)       *p++ = 'S';
    if (flags & SHF_INFO_LINK)     *p++ = 'I';
    if (flags & SHF_LINK_ORDER)    *p++ = 'L';
    if (flags & SHF_OS_NONCONFORMING) *p++ = 'O';
    if (flags & SHF_GROUP)         *p++ = 'G';
    if (flags & SHF_TLS)           *p++ = 'T';
    if (flags & SHF_EXCLUDE)       *p++ = 'E';

    // Null-terminate the string
    *p = '\0';

    return flags_string;
}

char *get_segment_type(unsigned long p_type)
{
    switch (p_type)
    {
    case PT_NULL:	return ("NULL");
    case PT_LOAD:	return ("LOAD");
    case PT_DYNAMIC:	return ("DYNAMIC");
    case PT_INTERP:	return ("INTERP");
    case PT_NOTE:	return ("NOTE");
    case PT_SHLIB:	return ("SHLIB");
    case PT_PHDR:	return ("PHDR");
    case PT_TLS:	return ("TLS");
    case PT_GNU_EH_FRAME:	return ("GNU_EH_FRAME");
    case PT_GNU_STACK:	return ("GNU_STACK");
    case PT_GNU_RELRO:  return ("GNU_RELRO");
    case 0x6464E550:	return ("LOOS+464e550");
    default:	return ("UNKNOWN");
    }
}