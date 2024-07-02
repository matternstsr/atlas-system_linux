#include "2-hreadelf.h"


/**
* switch_endianness_program_headers - Switches endianness of ELF program headers if necessary.
* @elf_header: Pointer to the ELF header structure.
* @index: Index of the program header to switch endianness.
*/
void switch_endianness_program_headers(elf_t *elf_header, size_t index)
{
    if (IS_BE(elf_header->e64)) {
        if (!IS_32(elf_header->e64)) {
            elf_header->p64[index].p_type = swap_endian_uint32(elf_header->p64[index].p_type);
            elf_header->p64[index].p_offset = swap_endian_uint64(elf_header->p64[index].p_offset);
            elf_header->p64[index].p_vaddr = swap_endian_uint64(elf_header->p64[index].p_vaddr);
            elf_header->p64[index].p_paddr = swap_endian_uint64(elf_header->p64[index].p_paddr);
            elf_header->p64[index].p_filesz = swap_endian_uint64(elf_header->p64[index].p_filesz);
            elf_header->p64[index].p_memsz = swap_endian_uint64(elf_header->p64[index].p_memsz);
            elf_header->p64[index].p_align = swap_endian_uint64(elf_header->p64[index].p_align);
            elf_header->p64[index].p_flags = swap_endian_uint32(elf_header->p64[index].p_flags);
        } else {
            elf_header->p32[index].p_type = swap_endian_uint32(elf_header->p32[index].p_type);
            elf_header->p32[index].p_offset = swap_endian_uint32(elf_header->p32[index].p_offset);
            elf_header->p32[index].p_vaddr = swap_endian_uint32(elf_header->p32[index].p_vaddr);
            elf_header->p32[index].p_paddr = swap_endian_uint32(elf_header->p32[index].p_paddr);
            elf_header->p32[index].p_filesz = swap_endian_uint32(elf_header->p32[index].p_filesz);
            elf_header->p32[index].p_memsz = swap_endian_uint32(elf_header->p32[index].p_memsz);
            elf_header->p32[index].p_align = swap_endian_uint32(elf_header->p32[index].p_align);
            elf_header->p32[index].p_flags = swap_endian_uint32(elf_header->p32[index].p_flags);
        }
    }
}

/**
* switch_all_endian - switches all endians if need be
* @h: address of our internal struct
*/
void switch_all_endian(elf_t *h)
{
    if (IS_BE(h->e64))
    {
        h->e64.e_machine = switch_2_byte(h->e64.e_machine);
        h->e64.e_version = swap_endian_uint32(h->e64.e_version);
        h->e64.e_type = switch_2_byte(h->e64.e_type);
        h->e64.e_flags = swap_endian_uint32(h->e64.e_flags);
        h->e64.e_ehsize = switch_2_byte(h->e64.e_ehsize);
        h->e64.e_phentsize = switch_2_byte(h->e64.e_phentsize);
        h->e64.e_phnum = switch_2_byte(h->e64.e_phnum);
        h->e64.e_shentsize = switch_2_byte(h->e64.e_shentsize);
        h->e64.e_shnum = switch_2_byte(h->e64.e_shnum);
        h->e64.e_shoff = swap_endian_uint64(h->e64.e_shoff);
        h->e64.e_phoff = swap_endian_uint64(h->e64.e_phoff);
        h->e64.e_entry = swap_endian_uint64(h->e64.e_entry);
        h->e64.e_shstrndx = switch_2_byte(h->e64.e_shstrndx);
    }
    if (IS_BE(h->e64) && IS_32(h->e64))
    {
        h->e32.e_machine = switch_2_byte(h->e32.e_machine);
        h->e32.e_version = swap_endian_uint32(h->e32.e_version);
        h->e32.e_type = switch_2_byte(h->e32.e_type);
        h->e32.e_flags = swap_endian_uint32(h->e32.e_flags);
        h->e32.e_ehsize = switch_2_byte(h->e32.e_ehsize);
        h->e32.e_phentsize = switch_2_byte(h->e32.e_phentsize);
        h->e32.e_phnum = switch_2_byte(h->e32.e_phnum);
        h->e32.e_shentsize = switch_2_byte(h->e32.e_shentsize);
        h->e32.e_shnum = switch_2_byte(h->e32.e_shnum);
        h->e32.e_shoff = swap_endian_uint32(h->e32.e_shoff);
        h->e32.e_phoff = swap_endian_uint32(h->e32.e_phoff);
        h->e32.e_entry = swap_endian_uint32(h->e32.e_entry);
        h->e32.e_shstrndx = switch_2_byte(h->e32.e_shstrndx);
    }
}

/**
* swap_endian_uint32 - Swaps endianness of a 32-bit unsigned integer.
* @value: The unsigned integer whose endianness to swap.
* Return: Unsigned integer with swapped endianness.
*/
unsigned int swap_endian_uint32(unsigned int value)
{
    return (((value >> 24) & 0x000000ff) |
            ((value >> 8)  & 0x0000ff00) |
            ((value << 8)  & 0x00ff0000) |
            ((value << 24) & 0xff000000));
}

/**
* swap_endian_uint64 - Swaps endianness of a 64-bit unsigned integer.
* @value: The unsigned long integer whose endianness to swap.
* Return: Unsigned long integer with swapped endianness.
*/
unsigned long swap_endian_uint64(unsigned long value)
{
    return (((value >> 56) & 0x00000000000000ff) |
            ((value >> 40) & 0x000000000000ff00) |
            ((value >> 24) & 0x0000000000ff0000) |
            ((value >> 8)  & 0x00000000ff000000) |
            ((value << 8)  & 0x000000ff00000000) |
            ((value << 24) & 0x0000ff0000000000) |
            ((value << 40) & 0x00ff000000000000) |
            ((value << 56) & 0xff00000000000000));
}

/**
* switch_endianness_sections - Switches endianness of ELF section headers if necessary.
* @elf_header: Pointer to the ELF header structure.
* @index: Index of the section header to switch endianness.
*/
void switch_endianness_sections(elf_t *elf_header, size_t index)
{
    if (IS_BE(elf_header->e64)) {
        if (!IS_32(elf_header->e64)) {
            elf_header->s64[index].sh_name = swap_endian_uint32(elf_header->s64[index].sh_name);
            elf_header->s64[index].sh_type = swap_endian_uint32(elf_header->s64[index].sh_type);
            elf_header->s64[index].sh_flags = swap_endian_uint64(elf_header->s64[index].sh_flags);
            elf_header->s64[index].sh_addr = swap_endian_uint64(elf_header->s64[index].sh_addr);
            elf_header->s64[index].sh_offset = swap_endian_uint64(elf_header->s64[index].sh_offset);
            elf_header->s64[index].sh_size = swap_endian_uint64(elf_header->s64[index].sh_size);
            elf_header->s64[index].sh_link = swap_endian_uint32(elf_header->s64[index].sh_link);
            elf_header->s64[index].sh_info = swap_endian_uint32(elf_header->s64[index].sh_info);
            elf_header->s64[index].sh_addralign = swap_endian_uint64(elf_header->s64[index].sh_addralign);
            elf_header->s64[index].sh_entsize = swap_endian_uint64(elf_header->s64[index].sh_entsize);
        } else {
            elf_header->s32[index].sh_name = swap_endian_uint32(elf_header->s32[index].sh_name);
            elf_header->s32[index].sh_type = swap_endian_uint32(elf_header->s32[index].sh_type);
            elf_header->s32[index].sh_flags = swap_endian_uint32(elf_header->s32[index].sh_flags);
            elf_header->s32[index].sh_addr = swap_endian_uint32(elf_header->s32[index].sh_addr);
            elf_header->s32[index].sh_offset = swap_endian_uint32(elf_header->s32[index].sh_offset);
            elf_header->s32[index].sh_size = swap_endian_uint32(elf_header->s32[index].sh_size);
            elf_header->s32[index].sh_link = swap_endian_uint32(elf_header->s32[index].sh_link);
            elf_header->s32[index].sh_info = swap_endian_uint32(elf_header->s32[index].sh_info);
            elf_header->s32[index].sh_addralign = swap_endian_uint32(elf_header->s32[index].sh_addralign);
            elf_header->s32[index].sh_entsize = swap_endian_uint32(elf_header->s32[index].sh_entsize);
        }
    }
}

