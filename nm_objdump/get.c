#include "hnm.h"     /* Include header file hnm.h */

/**
* get_section_type - return string type of section
* @sh_type: the section type
* Return: string name of type
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
		return (buf);     /* Return pointer to buf */
	}
}

/**
* get_section_flags - translates section flags to string
* @elf_header: address of elf header struct
* @i: the section index
* Return: legend-based string rep of flag
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
	return (buf);                       /* Return pointer to the flag string */
}

/**
* get_sym_type - gets the STT type based on symbol information
* @elf_header: the internal header struct (unused)
* @i: index of the current symbol in the symbol table
* Return: string representing the symbol type
*/
char *get_sym_type(elf_t *elf_header, size_t i)
{
	switch (YGET(i, st_info) & 0xf)
	{
		case STT_NOTYPE:  return ("NOTYPE");    /* No type */
		case STT_OBJECT:  return ("OBJECT");    /* Data object */
		case STT_FUNC:    return ("FUNC");      /* Function entry point */
		case STT_SECTION: return ("SECTION");   /* Section */
		case STT_FILE:    return ("FILE");    /* Source file symbol */
		case STT_COMMON:  return ("COMMON"); /* Uninitialized common block */
		case STT_TLS:     return ("TLS");    /* Thread-local storage symbol */
		case STT_NUM:     return ("NUM");    /* Number of defined types */
		case STT_LOOS:    return ("LOOS");   /* Start of OS-specific types */
		case STT_HIOS:    return ("HIOS");  /* End of OS-specific types */
		case STT_LOPROC:  return ("LOPROC"); /* Start of processor-spec types */
		case STT_HIPROC:  return ("HIPROC"); /* End of processor-spec types */
		default:          return ("UNKNOWN"); /* Unknown symbol type */
	}
}


/**
* get_sym_bind - gets the STB (symbol binding) type based on symbol information
* @elf_header: the internal ELF header struct (unused)
* @i: index of the current symbol in the symbol table
* Return: string representing the symbol binding type
*/
char *get_sym_bind(elf_t *elf_header, size_t i)
{
	switch (YGET(i, st_info) >> 4)
	{
		case STB_LOCAL:    return ("LOCAL");    /* Local symbol */
		case STB_GLOBAL:   return ("GLOBAL");   /* Global symbol */
		case STB_WEAK:     return ("WEAK");    /* Weak symbol */
		case STB_NUM:      return ("NUM");     /* Number of defined types */
		case STB_LOOS:     return ("LOOS");    /* Start of OS-specific types */
		case STB_HIOS:     return ("HIOS");  /* End of OS-specific types */
		case STB_LOPROC:   return ("LOPROC");/* Start of processor-spec types */
		case STB_HIPROC:   return ("HIPROC"); /* End of processor-spec types */
		default:           return ("UNKNWN"); /* Unknown symbol binding type */
	}
}

/**
* get_sym_visibility - gets the STV (symbol visibility) type
* based on symbol information
* @elf_header: the internal ELF header struct (unused)
* @i: index of the current symbol in the symbol table
* Return: string representing the symbol visibility type
*/
char *get_sym_visibility(elf_t *elf_header, size_t i)
{
	switch (YGET(i, st_other) & 0x3)
	{
		case STV_DEFAULT:    return ("DEFAULT");    /* Default visibility */
		case STV_INTERNAL:   return ("INTERNAL");   /* Internal visibility */
		case STV_HIDDEN:     return ("HIDDEN");     /* Hidden visibility */
		case STV_PROTECTED:  return ("PROTECTED");  /* Protected visibility */
		default:             return ("UNKNOWN");    /* Unknown visibility */
	}
}

/**
* get_nm_type64 - gets type of symbol in one character for 64-bit ELF
* @sym: the symbol struct (Elf64_Sym)
* @shdr: the array of section header structs (Elf64_Shdr)
* Return: the character representing the symbol type
*/
char get_nm_type64(Elf64_Sym sym, Elf64_Shdr *shdr)
{
	char c;  /* Variable to hold the symbol type character */

	/* Determine symbol type based on symbol information and section header */
	if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';  /* GNU unique symbol */
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK &&
			ELF64_ST_TYPE(sym.st_info) == STT_OBJECT)
		c = sym.st_shndx == SHN_UNDEF ? 'v' : 'V';  /* Weak object symbol */
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
		c = sym.st_shndx == SHN_UNDEF ? 'w' : 'W';  /* Weak symbol */
	else if (sym.st_shndx == SHN_UNDEF)
		c = 'U';  /* Undefined symbol */
	else if (sym.st_shndx == SHN_ABS)
		c = 'A';  /* Absolute symbol */
	else if (sym.st_shndx == SHN_COMMON)
		c = 'C';  /* Common symbol */
	else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS &&
			shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';  /* BSS (uninitialized data) symbol */
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
			shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
		c = 'R';  /* Read-only data symbol */
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
			shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';  /* Data symbol */
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
			shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';  /* Text (executable code) symbol */
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';  /* Dynamic symbol */
	else
		c = 'T';  /* Other symbol type */
	/* Convert to lowercase if local and not unknown */
	if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
		c += 32;
	return (c);  /* Return the symbol type character */
}

/**
* get_nm_type32 - gets type of symbol in one character for 32-bit ELF
* @sym: the symbol struct (Elf32_Sym)
* @shdr: the array of section header structs (Elf32_Shdr)
* Return: the character representing the symbol type
*/
char get_nm_type32(Elf32_Sym sym, Elf32_Shdr *shdr)
{
	char c;  /* Variable to hold the symbol type character */

	/* Determine symbol type based on symbol information and section header */
	if (ELF32_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';  /* GNU unique symbol */
	else if (ELF32_ST_BIND(sym.st_info) == STB_WEAK &&
			ELF32_ST_TYPE(sym.st_info) == STT_OBJECT)
		c = sym.st_shndx == SHN_UNDEF ? 'v' : 'V';  /* Weak object symbol */
	else if (ELF32_ST_BIND(sym.st_info) == STB_WEAK)
		c = sym.st_shndx == SHN_UNDEF ? 'w' : 'W';  /* Weak symbol */
	else if (sym.st_shndx == SHN_UNDEF)
		c = 'U';  /* Undefined symbol */
	else if (sym.st_shndx == SHN_ABS)
		c = 'A';  /* Absolute symbol */
	else if (sym.st_shndx == SHN_COMMON)
		c = 'C';  /* Common symbol */
	else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS &&
			shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';  /* BSS (uninitialized data) symbol */
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
			shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
		c = 'R';  /* Read-only data symbol */
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
			shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';  /* Data symbol */
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
			shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';  /* Text (executable code) symbol */
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';  /* Dynamic symbol */
	else
		c = 'T';  /* Other symbol type */
	/* Convert to lowercase if local and not unknown */
	if (ELF32_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
		c += 32;
	return (c);  /* Return the symbol type character */
}
