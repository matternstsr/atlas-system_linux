#include "2-hreadelf.h"

/**
* switch_endianness_program_headers - Switches ELF prog hdrs.
* @EH: Pointer to the ELF header structure.
* @index: Index of the program header to switch endianness.
*/
void switch_endianness_program_headers(elf_t *EH, size_t index)
{
	if (IS_BE(EH->e64))
	{
		if (!IS_32(EH->e64))
			switch_endianness_program_headers_64(EH, index);
		else
			switch_endianness_program_headers_32(EH, index);
	}
}

/**
* switch_endianness_program_headers_64 - Switch to 64-bit ELF prog hdrs.
* @EH: Pointer to the ELF header structure.
* @index: Index of the program header to switch endianness.
*/
void switch_endianness_program_headers_64(elf_t *EH, size_t index)
{
	EH->p64[index].p_type    = seu32(EH->p64[index].p_type);
	EH->p64[index].p_offset  = seu64(EH->p64[index].p_offset);
	EH->p64[index].p_vaddr   = seu64(EH->p64[index].p_vaddr);
	EH->p64[index].p_paddr   = seu64(EH->p64[index].p_paddr);
	EH->p64[index].p_filesz  = seu64(EH->p64[index].p_filesz);
	EH->p64[index].p_memsz   = seu64(EH->p64[index].p_memsz);
	EH->p64[index].p_align   = seu64(EH->p64[index].p_align);
	EH->p64[index].p_flags   = seu32(EH->p64[index].p_flags);
}

/**
* switch_endianness_program_headers_32 - Switch to 32-bit ELF prog hdrs.
* @EH: Pointer to the ELF header structure.
* @index: Index of the program header to switch endianness.
*/
void switch_endianness_program_headers_32(elf_t *EH, size_t index)
{
	EH->p32[index].p_type    = seu32(EH->p32[index].p_type);
	EH->p32[index].p_offset  = seu32(EH->p32[index].p_offset);
	EH->p32[index].p_vaddr   = seu32(EH->p32[index].p_vaddr);
	EH->p32[index].p_paddr   = seu32(EH->p32[index].p_paddr);
	EH->p32[index].p_filesz  = seu32(EH->p32[index].p_filesz);
	EH->p32[index].p_memsz   = seu32(EH->p32[index].p_memsz);
	EH->p32[index].p_align   = seu32(EH->p32[index].p_align);
	EH->p32[index].p_flags   = seu32(EH->p32[index].p_flags);
}
