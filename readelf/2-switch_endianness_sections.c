#include "2-hreadelf.h"

/**
* switch_endianness_sections - Switches ELF sec hdrs.
* @EH: Pointer to the ELF header structure.
* @index: Index of the section header to switch endianness.
*/
void switch_endianness_sections(elf_t *EH, size_t index)
{
	if (IS_BE(EH->e64))
	{
		if (!IS_32(EH->e64))
			switch_endianness_sections_64(EH, index);
		else
			switch_endianness_sections_32(EH, index);
	}
}


/**
* switch_endianness_sections_64 - Switch to 64-bit ELF sec hdrs.
* @EH: Pointer to the ELF header structure.
* @index: Index of the section header to switch endianness.
*/
void switch_endianness_sections_64(elf_t *EH, size_t index)
{
	EH->s64[index].sh_name       = seu32(EH->s64[index].sh_name);
	EH->s64[index].sh_type       = seu32(EH->s64[index].sh_type);
	EH->s64[index].sh_flags      = seu64(EH->s64[index].sh_flags);
	EH->s64[index].sh_addr       = seu64(EH->s64[index].sh_addr);
	EH->s64[index].sh_offset     = seu64(EH->s64[index].sh_offset);
	EH->s64[index].sh_size       = seu64(EH->s64[index].sh_size);
	EH->s64[index].sh_link       = seu32(EH->s64[index].sh_link);
	EH->s64[index].sh_info       = seu32(EH->s64[index].sh_info);
	EH->s64[index].sh_addralign  = seu64(EH->s64[index].sh_addralign);
	EH->s64[index].sh_entsize    = seu64(EH->s64[index].sh_entsize);
}

/**
* switch_endianness_sections_32 - Switch to 32-bit ELF sec hdrs.
* @EH: Pointer to the ELF header structure.
* @index: Index of the section header to switch endianness.
*/
void switch_endianness_sections_32(elf_t *EH, size_t index)
{
	EH->s32[index].sh_name       = seu32(EH->s32[index].sh_name);
	EH->s32[index].sh_type       = seu32(EH->s32[index].sh_type);
	EH->s32[index].sh_flags      = seu32(EH->s32[index].sh_flags);
	EH->s32[index].sh_addr       = seu32(EH->s32[index].sh_addr);
	EH->s32[index].sh_offset     = seu32(EH->s32[index].sh_offset);
	EH->s32[index].sh_size       = seu32(EH->s32[index].sh_size);
	EH->s32[index].sh_link       = seu32(EH->s32[index].sh_link);
	EH->s32[index].sh_info       = seu32(EH->s32[index].sh_info);
	EH->s32[index].sh_addralign  = seu32(EH->s32[index].sh_addralign);
	EH->s32[index].sh_entsize    = seu32(EH->s32[index].sh_entsize);
}
