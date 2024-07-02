#include "2-hreadelf.h"

/**
 * switch_endianness_sections - Switches endianness of ELF section headers if necessary.
 * @elf_header: Pointer to the ELF header structure.
 * @index: Index of the section header to switch endianness.
 */
void switch_endianness_sections(elf_t *elf_header, size_t index)
{
    if (IS_BE(elf_header->e64)) {
        if (!IS_32(elf_header->e64)) {
            switch_endianness_sections_64(elf_header, index);
        } else {
            switch_endianness_sections_32(elf_header, index);
        }
    }
}


/**
* switch_endianness_sections_64 - Switches endianness of 64-bit ELF section headers if necessary.
* @elf_header: Pointer to the ELF header structure.
* @index: Index of the section header to switch endianness.
*/
void switch_endianness_sections_64(elf_t *elf_header, size_t index)
{
	elf_header->s64[index].sh_name       = seu32(elf_header->s64[index].sh_name);
	elf_header->s64[index].sh_type       = seu32(elf_header->s64[index].sh_type);
	elf_header->s64[index].sh_flags      = swap_endian_uint64(elf_header->s64[index].sh_flags);
	elf_header->s64[index].sh_addr       = swap_endian_uint64(elf_header->s64[index].sh_addr);
	elf_header->s64[index].sh_offset     = swap_endian_uint64(elf_header->s64[index].sh_offset);
	elf_header->s64[index].sh_size       = swap_endian_uint64(elf_header->s64[index].sh_size);
	elf_header->s64[index].sh_link       = seu32(elf_header->s64[index].sh_link);
	elf_header->s64[index].sh_info       = seu32(elf_header->s64[index].sh_info);
	elf_header->s64[index].sh_addralign  = swap_endian_uint64(elf_header->s64[index].sh_addralign);
	elf_header->s64[index].sh_entsize    = swap_endian_uint64(elf_header->s64[index].sh_entsize);
}

/**
* switch_endianness_sections_32 - Switches endianness of 32-bit ELF section headers if necessary.
* @elf_header: Pointer to the ELF header structure.
* @index: Index of the section header to switch endianness.
*/
void switch_endianness_sections_32(elf_t *elf_header, size_t index)
{
	elf_header->s32[index].sh_name       = seu32(elf_header->s32[index].sh_name);
	elf_header->s32[index].sh_type       = seu32(elf_header->s32[index].sh_type);
	elf_header->s32[index].sh_flags      = seu32(elf_header->s32[index].sh_flags);
	elf_header->s32[index].sh_addr       = seu32(elf_header->s32[index].sh_addr);
	elf_header->s32[index].sh_offset     = seu32(elf_header->s32[index].sh_offset);
	elf_header->s32[index].sh_size       = seu32(elf_header->s32[index].sh_size);
	elf_header->s32[index].sh_link       = seu32(elf_header->s32[index].sh_link);
	elf_header->s32[index].sh_info       = seu32(elf_header->s32[index].sh_info);
	elf_header->s32[index].sh_addralign  = seu32(elf_header->s32[index].sh_addralign);
	elf_header->s32[index].sh_entsize    = seu32(elf_header->s32[index].sh_entsize);
}
