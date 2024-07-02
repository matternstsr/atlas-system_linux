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
            switch_endianness_program_headers_64(elf_header, index);
        } else {
            switch_endianness_program_headers_32(elf_header, index);
        }
    }
}

/**
 * switch_endianness_program_headers_64 - Switches endianness of 64-bit ELF program headers if necessary.
 * @elf_header: Pointer to the ELF header structure.
 * @index: Index of the program header to switch endianness.
 */
void switch_endianness_program_headers_64(elf_t *elf_header, size_t index)
{
    elf_header->p64[index].p_type    = swap_endian_uint32(elf_header->p64[index].p_type);
    elf_header->p64[index].p_offset  = swap_endian_uint64(elf_header->p64[index].p_offset);
    elf_header->p64[index].p_vaddr   = swap_endian_uint64(elf_header->p64[index].p_vaddr);
    elf_header->p64[index].p_paddr   = swap_endian_uint64(elf_header->p64[index].p_paddr);
    elf_header->p64[index].p_filesz  = swap_endian_uint64(elf_header->p64[index].p_filesz);
    elf_header->p64[index].p_memsz   = swap_endian_uint64(elf_header->p64[index].p_memsz);
    elf_header->p64[index].p_align   = swap_endian_uint64(elf_header->p64[index].p_align);
    elf_header->p64[index].p_flags   = swap_endian_uint32(elf_header->p64[index].p_flags);
}

/**
 * switch_endianness_program_headers_32 - Switches endianness of 32-bit ELF program headers if necessary.
 * @elf_header: Pointer to the ELF header structure.
 * @index: Index of the program header to switch endianness.
 */
void switch_endianness_program_headers_32(elf_t *elf_header, size_t index)
{
    elf_header->p32[index].p_type    = swap_endian_uint32(elf_header->p32[index].p_type);
    elf_header->p32[index].p_offset  = swap_endian_uint32(elf_header->p32[index].p_offset);
    elf_header->p32[index].p_vaddr   = swap_endian_uint32(elf_header->p32[index].p_vaddr);
    elf_header->p32[index].p_paddr   = swap_endian_uint32(elf_header->p32[index].p_paddr);
    elf_header->p32[index].p_filesz  = swap_endian_uint32(elf_header->p32[index].p_filesz);
    elf_header->p32[index].p_memsz   = swap_endian_uint32(elf_header->p32[index].p_memsz);
    elf_header->p32[index].p_align   = swap_endian_uint32(elf_header->p32[index].p_align);
    elf_header->p32[index].p_flags   = swap_endian_uint32(elf_header->p32[index].p_flags);
}
