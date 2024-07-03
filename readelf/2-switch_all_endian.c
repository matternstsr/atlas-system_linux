#include "2-hreadelf.h"


/**
* switch_all_endian - switches all endians if need be
* @h: address of our internal struct
*/
void switch_all_endian(elf_t *h)
{
	if (IS_BE(h->e64))
	{
		h->e64.e_machine = switch_2_byte(h->e64.e_machine);
		h->e64.e_version = seu32(h->e64.e_version);
		h->e64.e_type = switch_2_byte(h->e64.e_type);
		h->e64.e_flags = seu32(h->e64.e_flags);
		h->e64.e_ehsize = switch_2_byte(h->e64.e_ehsize);
		h->e64.e_phentsize = switch_2_byte(h->e64.e_phentsize);
		h->e64.e_phnum = switch_2_byte(h->e64.e_phnum);
		h->e64.e_shentsize = switch_2_byte(h->e64.e_shentsize);
		h->e64.e_shnum = switch_2_byte(h->e64.e_shnum);
		h->e64.e_shoff = seu64(h->e64.e_shoff);
		h->e64.e_phoff = seu64(h->e64.e_phoff);
		h->e64.e_entry = seu64(h->e64.e_entry);
		h->e64.e_shstrndx = switch_2_byte(h->e64.e_shstrndx);
	}
	if (IS_BE(h->e64) && IS_32(h->e64))
	{
		h->e32.e_machine = switch_2_byte(h->e32.e_machine);
		h->e32.e_version = seu32(h->e32.e_version);
		h->e32.e_type = switch_2_byte(h->e32.e_type);
		h->e32.e_flags = seu32(h->e32.e_flags);
		h->e32.e_ehsize = switch_2_byte(h->e32.e_ehsize);
		h->e32.e_phentsize = switch_2_byte(h->e32.e_phentsize);
		h->e32.e_phnum = switch_2_byte(h->e32.e_phnum);
		h->e32.e_shentsize = switch_2_byte(h->e32.e_shentsize);
		h->e32.e_shnum = switch_2_byte(h->e32.e_shnum);
		h->e32.e_shoff = seu32(h->e32.e_shoff);
		h->e32.e_phoff = seu32(h->e32.e_phoff);
		h->e32.e_entry = seu32(h->e32.e_entry);
		h->e32.e_shstrndx = switch_2_byte(h->e32.e_shstrndx);
	}
}

/**
* seu32 - Swaps endianness of a 32-bit unsigned integer.
* @value: The unsigned integer whose endianness to swap.
* Return: Unsigned integer with swapped endianness.
*/
unsigned int seu32(unsigned int value)
{
	return (((value >> 24) & 0x000000ff) |
			((value >> 8)  & 0x0000ff00) |
			((value << 8)  & 0x00ff0000) |
			((value << 24) & 0xff000000));
}

/**
* seu64 - Swaps endianness of a 64-bit unsigned integer.
* @value: The unsigned long integer whose endianness to swap.
* Return: Unsigned long integer with swapped endianness.
*/
unsigned long seu64(unsigned long value)
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
