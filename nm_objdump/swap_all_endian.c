#include "hnm.h"

/**
 * swap_all_endian - switches all endians if need be
 * @h: address of our internal struct
 */
void swap_all_endian(elf_t *h)
{
	if (Is_BigE(h->e64))  /* Check if ELF header is big-endian */
	{
		h->e64.e_machine = swap_endian_uint16(h->e64.e_machine);/*machine */
		h->e64.e_version = swap_endian_uint32(h->e64.e_version);/*version */
		h->e64.e_type = swap_endian_uint16(h->e64.e_type);/*type */
		h->e64.e_flags = swap_endian_uint32(h->e64.e_flags);/*flags */
		h->e64.e_ehsize = swap_endian_uint16(h->e64.e_ehsize);/*elf hdr size */
		h->e64.e_phentsize = swap_endian_uint16(h->e64.e_phentsize);/*p hdr sz*/
		h->e64.e_phnum = swap_endian_uint16(h->e64.e_phnum);/*nump headers */
		h->e64.e_shentsize = swap_endian_uint16(h->e64.e_shentsize);/*sechdrsz*/
		h->e64.e_shnum = swap_endian_uint16(h->e64.e_shnum);/*nump sec hdrs */
		h->e64.e_shoff = swap_endian_uint64(h->e64.e_shoff);/*sec hdr tbl off */
		h->e64.e_phoff = swap_endian_uint64(h->e64.e_phoff);/*prog hdr tbl off*/
		h->e64.e_entry = swap_endian_uint64(h->e64.e_entry);/*entry point */
		h->e64.e_shstrndx = swap_endian_uint16(h->e64.e_shstrndx);
		/*section header string table index */
	}
	if (Is_BigE(h->e64) && IS_32(h->e64))
	/* Check if ELF header is big-endian and 32-bit */
	{
		h->e32.e_machine = swap_endian_uint16(h->e32.e_machine);/*machine */
		h->e32.e_version = swap_endian_uint32(h->e32.e_version);/*version */
		h->e32.e_type = swap_endian_uint16(h->e32.e_type);/*type */
		h->e32.e_flags = swap_endian_uint32(h->e32.e_flags);/*flags */
		h->e32.e_ehsize = swap_endian_uint16(h->e32.e_ehsize);/*elf hdr size */
		h->e32.e_phentsize = swap_endian_uint16(h->e32.e_phentsize);/*p hdr sz*/
		h->e32.e_phnum = swap_endian_uint16(h->e32.e_phnum);/*nump headers */
		h->e32.e_shentsize = swap_endian_uint16(h->e32.e_shentsize);/*sechdrsz*/
		h->e32.e_shnum = swap_endian_uint16(h->e32.e_shnum);/*nump sec hdrs */
		h->e32.e_shoff = swap_endian_uint32(h->e32.e_shoff);/*sec hdr tbl off */
		h->e32.e_phoff = swap_endian_uint32(h->e32.e_phoff);/*prog hdr tbl off*/
		h->e32.e_entry = swap_endian_uint32(h->e32.e_entry);/*entry point */
		h->e32.e_shstrndx = swap_endian_uint16(h->e32.e_shstrndx);
		/*section header string table index */
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
		h->s64[i].sh_name = swap_endian_uint32(h->s64[i].sh_name);/*sec nm off*/
		h->s64[i].sh_type = swap_endian_uint32(h->s64[i].sh_type);/*sec type */
		h->s64[i].sh_flags = swap_endian_uint64(h->s64[i].sh_flags);/*secflags*/
		h->s64[i].sh_addr = swap_endian_uint64(h->s64[i].sh_addr);/*sec viradd*/
		h->s64[i].sh_offset = swap_endian_uint64(h->s64[i].sh_offset);/*secoff*/
		h->s64[i].sh_size = swap_endian_uint64(h->s64[i].sh_size);/*sec size */
		h->s64[i].sh_link = swap_endian_uint32(h->s64[i].sh_link);/*sec link */
		h->s64[i].sh_info = swap_endian_uint32(h->s64[i].sh_info);/*sec info */
		h->s64[i].sh_addralign = swap_endian_uint64(h->s64[i].sh_addralign);
		/*section address alignment */
		h->s64[i].sh_entsize = swap_endian_uint64(h->s64[i].sh_entsize);
		/*section entry size */
	}
	if (Is_BigE(h->e64) && IS_32(h->e64))
	/* Check if ELF header is big-endian and 32-bit */
	{
		h->s32[i].sh_name = swap_endian_uint32(h->s32[i].sh_name);/*sec nm off*/
		h->s32[i].sh_type = swap_endian_uint32(h->s32[i].sh_type);/*sec type */
		h->s32[i].sh_flags = swap_endian_uint32(h->s32[i].sh_flags);/*secflags*/
		h->s32[i].sh_addr = swap_endian_uint32(h->s32[i].sh_addr);/*sec viradd*/
		h->s32[i].sh_offset = swap_endian_uint32(h->s32[i].sh_offset);/*secoff*/
		h->s32[i].sh_size = swap_endian_uint32(h->s32[i].sh_size);/*sec size */
		h->s32[i].sh_link = swap_endian_uint32(h->s32[i].sh_link);/*sec link */
		h->s32[i].sh_info = swap_endian_uint32(h->s32[i].sh_info);/*sec info */
		h->s32[i].sh_addralign = swap_endian_uint32(h->s32[i].sh_addralign);
		/*section address alignment */
		h->s32[i].sh_entsize = swap_endian_uint32(h->s32[i].sh_entsize);
		/*section entry size */
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
		h->p64[i].p_type = swap_endian_uint32(h->p64[i].p_type);/*proghdrtype*/
		h->p64[i].p_offset = swap_endian_uint64(h->p64[i].p_offset);/*prohdoff*/
		h->p64[i].p_vaddr = swap_endian_uint64(h->p64[i].p_vaddr);/*prhdrviadd*/
		h->p64[i].p_paddr = swap_endian_uint64(h->p64[i].p_paddr);/*prhdrphadd*/
		h->p64[i].p_filesz = swap_endian_uint64(h->p64[i].p_filesz);/*prhdrfls*/
		h->p64[i].p_memsz = swap_endian_uint64(h->p64[i].p_memsz);/*prohdrmems*/
		h->p64[i].p_align = swap_endian_uint64(h->p64[i].p_align);/*prohdralig*/
		h->p64[i].p_flags = swap_endian_uint32(h->p64[i].p_flags);/*prohdrflgs*/
	}
	if (Is_BigE(h->e64) && IS_32(h->e64))
	/* Check if ELF header is big-endian and 32-bit*/
	{
		h->p32[i].p_type = swap_endian_uint32(h->p32[i].p_type);/*proghdrtype*/
		h->p32[i].p_offset = swap_endian_uint32(h->p32[i].p_offset);/*prohdoff*/
		h->p32[i].p_vaddr = swap_endian_uint32(h->p32[i].p_vaddr);/*prhdrviadd*/
		h->p32[i].p_paddr = swap_endian_uint32(h->p32[i].p_paddr);/*prhdrphadd*/
		h->p32[i].p_filesz = swap_endian_uint32(h->p32[i].p_filesz);/*prhdrfls*/
		h->p32[i].p_memsz = swap_endian_uint32(h->p32[i].p_memsz);/*prohdrmems*/
		h->p32[i].p_align = swap_endian_uint32(h->p32[i].p_align);/*prohdralig*/
		h->p32[i].p_flags = swap_endian_uint32(h->p32[i].p_flags);/*prohdrflgs*/
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
		/*symbol name offset */
		h->y64[i].st_shndx = swap_endian_uint16(h->y64[i].st_shndx);
		/*symbol section index */
		h->y64[i].st_value = swap_endian_uint64(h->y64[i].st_value);
		/*symbol value */
		h->y64[i].st_size = swap_endian_uint64(h->y64[i].st_size);
		/*symbol size */
	}
	if (Is_BigE(h->e64) && IS_32(h->e64))
	/* Check if ELF header is big-endian and 32-bit */
	{
		h->y32[i].st_name = swap_endian_uint32(h->y32[i].st_name);
		/*symbol name offset */
		h->y32[i].st_shndx = swap_endian_uint16(h->y32[i].st_shndx);
		/*symbol section index */
		h->y32[i].st_value = swap_endian_uint32(h->y32[i].st_value);
		/*symbol value */
		h->y32[i].st_size = swap_endian_uint32(h->y32[i].st_size);
		/*symbol size */
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
		return;
	for (i = 0; i < versym_size; i++)
		versym[i] = swap_endian_uint16(versym[i]);
	/*version symbols */
	for (i = 0; i < verneed_size; i++)
	{
		verneed[i].vn_version = swap_endian_uint16(verneed[i].vn_version);
		/*version needed entry version */
		verneed[i].vn_cnt = swap_endian_uint16(verneed[i].vn_cnt);
		/*version needed entry count */
		verneed[i].vn_file = swap_endian_uint32(verneed[i].vn_file);
		/*version needed entry file */
		verneed[i].vn_aux = swap_endian_uint32(verneed[i].vn_aux);
		/*version needed entry aux */
		verneed[i].vn_next = swap_endian_uint32(verneed[i].vn_next);
		/*version needed entry next */
	}
	(void)versym;  /* Suppress unused parameter warning */
}
