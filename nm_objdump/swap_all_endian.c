#include "hnm.h"

/**
 * swap_all_endian - switches all endians if need be
 * @h: address of our internal struct
 */
void swap_all_endian(elf_t *h)
{
	if (Is_BigE(h->e64))  /* Check if ELF header is big-endian */
	{
		h->e64.e_machine = swap_endian_uint16(h->e64.e_machine);
		/* Swap endianness of machine */
		h->e64.e_version = swap_endian_uint32(h->e64.e_version);
		/* Swap endianness of version */
		h->e64.e_type = swap_endian_uint16(h->e64.e_type);
		/* Swap endianness of type */
		h->e64.e_flags = swap_endian_uint32(h->e64.e_flags);
		/* Swap endianness of flags */
		h->e64.e_ehsize = swap_endian_uint16(h->e64.e_ehsize);
		/* Swap endianness of ELF header size */
		h->e64.e_phentsize = swap_endian_uint16(h->e64.e_phentsize);
		/* Swap endianness of program header entry size */
		h->e64.e_phnum = swap_endian_uint16(h->e64.e_phnum);
		/* Swap endianness of number of program headers */
		h->e64.e_shentsize = swap_endian_uint16(h->e64.e_shentsize);
		/* Swap endianness of section header entry size */
		h->e64.e_shnum = swap_endian_uint16(h->e64.e_shnum);
		/* Swap endianness of number of section headers */
		h->e64.e_shoff = swap_endian_uint64(h->e64.e_shoff);
		/* Swap endianness of section header table offset */
		h->e64.e_phoff = swap_endian_uint64(h->e64.e_phoff);
		/* Swap endianness of program header table offset */
		h->e64.e_entry = swap_endian_uint64(h->e64.e_entry);
		/* Swap endianness of entry point */
		h->e64.e_shstrndx = swap_endian_uint16(h->e64.e_shstrndx);
		/* Swap endianness of section header string table index */
	}
	if (Is_BigE(h->e64) && IS_32(h->e64))
	/* Check if ELF header is big-endian and 32-bit */
	{
		h->e32.e_machine = swap_endian_uint16(h->e32.e_machine);
		/* Swap endianness of machine */
		h->e32.e_version = swap_endian_uint32(h->e32.e_version);
		/* Swap endianness of version */
		h->e32.e_type = swap_endian_uint16(h->e32.e_type);
		/* Swap endianness of type */
		h->e32.e_flags = swap_endian_uint32(h->e32.e_flags);
		/* Swap endianness of flags */
		h->e32.e_ehsize = swap_endian_uint16(h->e32.e_ehsize);
		/* Swap endianness of ELF header size */
		h->e32.e_phentsize = swap_endian_uint16(h->e32.e_phentsize);
		/* Swap endianness of program header entry size */
		h->e32.e_phnum = swap_endian_uint16(h->e32.e_phnum);
		/* Swap endianness of number of program headers */
		h->e32.e_shentsize = swap_endian_uint16(h->e32.e_shentsize);
		/* Swap endianness of section header entry size */
		h->e32.e_shnum = swap_endian_uint16(h->e32.e_shnum);
		/* Swap endianness of number of section headers */
		h->e32.e_shoff = swap_endian_uint32(h->e32.e_shoff);
		/* Swap endianness of section header table offset */
		h->e32.e_phoff = swap_endian_uint32(h->e32.e_phoff);
		/* Swap endianness of program header table offset */
		h->e32.e_entry = swap_endian_uint32(h->e32.e_entry);
		/* Swap endianness of entry point */
		h->e32.e_shstrndx = swap_endian_uint16(h->e32.e_shstrndx);
		/* Swap endianness of section header string table index */
	}
}

/**
 * swap_all_endian_section - switches all endians if need be
 * @h: address of our internal struct
 * @i: index of this section
 */
void swap_all_endian_section(elf_t *h, size_t i)
{
	if (Is_BigE(h->e64) && !IS_32(h->e64))
	/* Check if ELF header is big-endian and 64-bit */
	{
		h->s64[i].sh_name = swap_endian_uint32(h->s64[i].sh_name);
		/* Swap endianness of section name offset */
		h->s64[i].sh_type = swap_endian_uint32(h->s64[i].sh_type);
		/* Swap endianness of section type */
		h->s64[i].sh_flags = swap_endian_uint64(h->s64[i].sh_flags);
		/* Swap endianness of section flags */
		h->s64[i].sh_addr = swap_endian_uint64(h->s64[i].sh_addr);
		/* Swap endianness of section virtual address */
		h->s64[i].sh_offset = swap_endian_uint64(h->s64[i].sh_offset);
		/* Swap endianness of section offset */
		h->s64[i].sh_size = swap_endian_uint64(h->s64[i].sh_size);
		/* Swap endianness of section size */
		h->s64[i].sh_link = swap_endian_uint32(h->s64[i].sh_link);
		/* Swap endianness of section link */
		h->s64[i].sh_info = swap_endian_uint32(h->s64[i].sh_info);
		/* Swap endianness of section info */
		h->s64[i].sh_addralign = swap_endian_uint64(h->s64[i].sh_addralign);
		/* Swap endianness of section address alignment */
		h->s64[i].sh_entsize = swap_endian_uint64(h->s64[i].sh_entsize);
		/* Swap endianness of section entry size */
	}
	if (Is_BigE(h->e64) && IS_32(h->e64))
	/* Check if ELF header is big-endian and 32-bit */
	{
		h->s32[i].sh_name = swap_endian_uint32(h->s32[i].sh_name);
		/* Swap endianness of section name offset */
		h->s32[i].sh_type = swap_endian_uint32(h->s32[i].sh_type);
		/* Swap endianness of section type */
		h->s32[i].sh_flags = swap_endian_uint32(h->s32[i].sh_flags);
		/* Swap endianness of section flags */
		h->s32[i].sh_addr = swap_endian_uint32(h->s32[i].sh_addr);
		/* Swap endianness of section virtual address */
		h->s32[i].sh_offset = swap_endian_uint32(h->s32[i].sh_offset);
		/* Swap endianness of section offset */
		h->s32[i].sh_size = swap_endian_uint32(h->s32[i].sh_size);
		/* Swap endianness of section size */
		h->s32[i].sh_link = swap_endian_uint32(h->s32[i].sh_link);
		/* Swap endianness of section link */
		h->s32[i].sh_info = swap_endian_uint32(h->s32[i].sh_info);
		/* Swap endianness of section info */
		h->s32[i].sh_addralign = swap_endian_uint32(h->s32[i].sh_addralign);
		/* Swap endianness of section address alignment */
		h->s32[i].sh_entsize = swap_endian_uint32(h->s32[i].sh_entsize);
		/* Swap endianness of section entry size */
	}
}


/**
 * swap_all_endian_program - switches all endians if need be
 * @h: address of our internal struct
 * @i: index of this section
 */
void swap_all_endian_program(elf_t *h, size_t i)
{
	if (Is_BigE(h->e64) && !IS_32(h->e64))
	/* Check if ELF header is big-endian and 64-bit */
	{
		h->p64[i].p_type = swap_endian_uint32(h->p64[i].p_type);
		/* Swap endianness of program header type */
		h->p64[i].p_offset = swap_endian_uint64(h->p64[i].p_offset);
		/* Swap endianness of program header offset */
		h->p64[i].p_vaddr = swap_endian_uint64(h->p64[i].p_vaddr);
		/* Swap endianness of program header virtual address */
		h->p64[i].p_paddr = swap_endian_uint64(h->p64[i].p_paddr);
		/* Swap endianness of program header physical address */
		h->p64[i].p_filesz = swap_endian_uint64(h->p64[i].p_filesz);
		/* Swap endianness of program header file size */
		h->p64[i].p_memsz = swap_endian_uint64(h->p64[i].p_memsz);
		/* Swap endianness of program header memory size */
		h->p64[i].p_align = swap_endian_uint64(h->p64[i].p_align);
		/* Swap endianness of program header alignment */
		h->p64[i].p_flags = swap_endian_uint32(h->p64[i].p_flags);
		/* Swap endianness of program header flags */
	}
	if (Is_BigE(h->e64) && IS_32(h->e64))
	/* Check if ELF header is big-endian and 32-bit */
	{
		h->p32[i].p_type = swap_endian_uint32(h->p32[i].p_type);
		/* Swap endianness of program header type */
		h->p32[i].p_offset = swap_endian_uint32(h->p32[i].p_offset);
		/* Swap endianness of program header offset */
		h->p32[i].p_vaddr = swap_endian_uint32(h->p32[i].p_vaddr);
		/* Swap endianness of program header virtual address */
		h->p32[i].p_paddr = swap_endian_uint32(h->p32[i].p_paddr);
		/* Swap endianness of program header physical address */
		h->p32[i].p_filesz = swap_endian_uint32(h->p32[i].p_filesz);
		/* Swap endianness of program header file size */
		h->p32[i].p_memsz = swap_endian_uint32(h->p32[i].p_memsz);
		/* Swap endianness of program header memory size */
		h->p32[i].p_align = swap_endian_uint32(h->p32[i].p_align);
		/* Swap endianness of program header alignment */
		h->p32[i].p_flags = swap_endian_uint32(h->p32[i].p_flags);
		/* Swap endianness of program header flags */
	}
}

/**
 * swap_all_endian_symbol - switches all endians if need be
 * @h: address of our internal struct
 * @i: index of this section
 */
void swap_all_endian_symbol(elf_t *h, size_t i)
{
	if (Is_BigE(h->e64) && !IS_32(h->e64))
	/* Check if ELF header is big-endian and 64-bit */
	{
		h->y64[i].st_name = swap_endian_uint32(h->y64[i].st_name);
		/* Swap endianness of symbol name offset */
		h->y64[i].st_shndx = swap_endian_uint16(h->y64[i].st_shndx);
		/* Swap endianness of symbol section index */
		h->y64[i].st_value = swap_endian_uint64(h->y64[i].st_value);
		/* Swap endianness of symbol value */
		h->y64[i].st_size = swap_endian_uint64(h->y64[i].st_size);
		/* Swap endianness of symbol size */
	}
	if (Is_BigE(h->e64) && IS_32(h->e64))
	/* Check if ELF header is big-endian and 32-bit */
	{
		h->y32[i].st_name = swap_endian_uint32(h->y32[i].st_name);
		/* Swap endianness of symbol name offset */
		h->y32[i].st_shndx = swap_endian_uint16(h->y32[i].st_shndx);
		/* Swap endianness of symbol section index */
		h->y32[i].st_value = swap_endian_uint32(h->y32[i].st_value);
		/* Swap endianness of symbol value */
		h->y32[i].st_size = swap_endian_uint32(h->y32[i].st_size);
		/* Swap endianness of symbol size */
	}
	(void)i;  /* Suppress unused parameter warning */
}

/**
 * swap_all_endian_verneed - switches all endians if need be
 * @h: address of our internal struct
 * @versym: array of version symbols
 * @versym_size: number of elements in versym array
 * @verneed: array of version needed entries
 * @verneed_size: number of elements in verneed array
 */
void swap_all_endian_verneed(elf_t *h, uint16_t *versym, size_t versym_size,
	Elf64_Verneed *verneed, size_t verneed_size)
{
	size_t i = 0;

	if (!Is_BigE(h->e64))  /* Check if ELF header is big-endian */
		Return;
	for (i = 0; i < versym_size; i++)
		versym[i] = swap_endian_uint16(versym[i]);
	/* Swap endianness of version symbols */
	for (i = 0; i < verneed_size; i++)
	{
		verneed[i].vn_version = swap_endian_uint16(verneed[i].vn_version);
		/* Swap endianness of version needed entry version */
		verneed[i].vn_cnt = swap_endian_uint16(verneed[i].vn_cnt);
		/* Swap endianness of version needed entry count */
		verneed[i].vn_file = swap_endian_uint32(verneed[i].vn_file);
		/* Swap endianness of version needed entry file */
		verneed[i].vn_aux = swap_endian_uint32(verneed[i].vn_aux);
		/* Swap endianness of version needed entry aux */
		verneed[i].vn_next = swap_endian_uint32(verneed[i].vn_next);
		/* Swap endianness of version needed entry next */
	}
	(void)versym;  /* Suppress unused parameter warning */
}
