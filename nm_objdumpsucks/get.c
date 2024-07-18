#include "hnm.h"

/**
* get_section_type - return string type of section
* @sh_type: the section type
* return: string name of type
*/
char *get_section_type(unsigned int sh_type)
{
	static char buf[32];   /* Static buffer to store result */

	switch (sh_type)
	{
	case SHT_NULL:          return ("NULL");
	case SHT_PROGBITS:      return ("PROGBITS");
	case SHT_SYMTAB:        return ("SYMTAB");
	case SHT_STRTAB:        return ("STRTAB");
	case SHT_RELA:          return ("RELA");
	case SHT_HASH:          return ("HASH");
	case SHT_DYNAMIC:       return ("DYNAMIC");
	case SHT_NOTE:          return ("NOTE");
	case SHT_NOBITS:        return ("NOBITS");
	case SHT_REL:           return ("REL");
	case SHT_SHLIB:         return ("SHLIB");
	case SHT_DYNSYM:        return ("DYNSYM");
	case SHT_INIT_ARRAY:    return ("INIT_ARRAY");
	case SHT_FINI_ARRAY:    return ("FINI_ARRAY");
	case SHT_PREINIT_ARRAY: return ("PREINIT_ARRAY");
	case SHT_GNU_HASH:      return ("GNU_HASH");
	case SHT_GROUP:         return ("GROUP");
	case SHT_SYMTAB_SHNDX:  return ("SYMTAB SECTION INDICIES");
	case SHT_GNU_verdef:    return ("VERDEF");
	case SHT_GNU_verneed:   return ("VERNEED");
	case SHT_GNU_versym:    return ("VERSYM");
	case 0x6ffffff0:        return ("VERSYM");
	case 0x6ffffffc:        return ("VERDEF");
	case 0x7ffffffd:        return ("AUXILIARY");
	case 0x7fffffff:        return ("FILTER");
	case 0x6ffffff1:        return ("LOOS+ffffff1");
	case 0x6ffffff3:        return ("LOOS+ffffff3");
	case SHT_GNU_LIBLIST:   return ("GNU_LIBLIST");
	default:
		sprintf(buf, "%08x: <unknown>", sh_type);
		/*Fmt unknown sec type into buf */
		return (buf);     /* return pointer to buf */
	}
}

/**
* get_section_flags - translates section flags to string
* @elf_header: address of elf header struct
* @i: the section index
* return: legend-based string rep of flag
*/
char *get_section_flags(elf_t *elf_header, size_t i)
{
	static char buf[32];                /* Static buffer to store flag string */
	unsigned long flags = SGET(i, sh_flags);   /* Retrieve section flags */
	char *p = buf;                      /* Pointer to traverse the buffer */

	/* Check and append flag characters based on flag bits */
	if (flags & SHF_WRITE)              /* Writeable */
		*p++ = 'W';
	if (flags & SHF_ALLOC)              /* Allocatable */
		*p++ = 'A';
	if (flags & SHF_EXECINSTR)          /* Executable */
		*p++ = 'X';
	if (flags & SHF_MERGE)              /* Mergable */
		*p++ = 'M';
	if (flags & SHF_STRINGS)            /* Contains strings */
		*p++ = 'S';
	if (flags & SHF_INFO_LINK)          /* Link information */
		*p++ = 'I';
	if (flags & SHF_LINK_ORDER)         /* Link order */
		*p++ = 'L';
	if (flags & SHF_OS_NONCONFORMING)   /* Non-conforming OS */
		*p++ = 'O';
	if (flags & SHF_GROUP)              /* Member of a section group */
		*p++ = 'G';
	if (flags & SHF_TLS)                /* Thread-local storage */
		*p++ = 'T';
	if (flags & SHF_EXCLUDE)            /* Excluded from executable */
		*p++ = 'E';
	/* if (flags & SHF_COMPRESSED) *p = 'C'; opt compressed (not enabled) */
	*p = 0;                             /* Null-terminate the string */
	return (buf);                       /* return pointer to the flag string */
}
