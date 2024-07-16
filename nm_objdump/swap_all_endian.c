#include "hnm.h"

/**
 * swap_all_endian - switches all endians if need be
 * @h: address of our internal struct
 */
void swap_all_endian(elf_t *h)
{
	(void)h;
}


/**
 * swap_all_endian_section - switches all endians if need be
 * @h: address of our internal struct
 * @i: index of this section
 */
void swap_all_endian_section(elf_t *h, size_t i)
{
	(void)h;
	(void)i;
}

/**
 * swap_all_endian_program - switches all endians if need be
 * @h: address of our internal struct
 * @i: index of this section
 */
void swap_all_endian_program(elf_t *h, size_t i)
{
	(void)h;
	(void)i;
}

/**
 * swap_all_endian_symbol - switches all endians if need be
 * @h: address of our internal struct
 * @i: index of this section
 */
void swap_all_endian_symbol(elf_t *h, size_t i)
{
	(void)h;
	(void)i;
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
	(void)h;
	(void)versym;
	(void)versym_size;
	(void)verneed;
	(void)verneed_size;
}
