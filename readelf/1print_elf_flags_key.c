#include "1-hreadelf.h"

/**
 * print_elf_flags_key - Print key to ELF flags
 *
 * This function prints a key explaining the meaning of various flags
 * used in ELF (Executable and Linkable Format) files.
 *
 * It prints:
 *  - W (write), A (alloc), X (execute), M (merge), S (strings), l (large)
 *  - I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
 *  - O (extra OS processing required), o (OS specific), p (processor specific)
 */
void print_elf_flags_key(void)
{
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings)");
	printf(", l (large)\n");
	printf("  I (info), L (link order), G (group), ");
	printf("T (TLS), E (exclude), x (unknown)\n");
	printf("  O (extra OS processing required) ");
	printf("o (OS specific), p (processor specific)\n");
}

/**
 * print_flag_key - Print key to flags
 *
 * This function prints a key explaining the meaning of various flags.
 *
 * It prints:
 *  - W (write), A (alloc), X (execute), M (merge), S (strings)
 *  - I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
 *  - O (extra OS processing required), o (OS specific), p (processor specific)
 */
void print_flag_key(void)
{
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
	printf("  I (info), L (link order), G (group), ");
	printf("T (TLS), E (exclude), x (unknown)\n");
	printf("  O (extra OS processing required) ");
	printf("o (OS specific), p (processor specific)\n");
}