#include "hnm.h"

/**
* get_sym_type - gets the STT type based on symbol information
* @elf_header: the internal header struct (unused)
* @i: index of the current symbol in the symbol table
* return: string representing the symbol type
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
* return: string representing the symbol binding type
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
* return: string representing the symbol visibility type
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
* return: the character representing the symbol type
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
	return (c);  /* return the symbol type character */
}

/**
* get_nm_type32 - gets type of symbol in one character for 32-bit ELF
* @sym: the symbol struct (Elf32_Sym)
* @shdr: the array of section header structs (Elf32_Shdr)
* return: the character representing the symbol type
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
	return (c);  /* return the symbol type character */
}
