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
	const char *a = "W (write)";
	const char *b = "A (alloc)";
	const char *c = "X (execute)";
	const char *d = "M (merge)";
	const char *e = "I (info)";
	const char *f = "L (link order)";
	const char *g = "G (group)";
	const char *h = "T (TLS)";
	const char *i = "O (extra OS processing)";
	const char *j = "S (strings)";
	const char *k = "l (large)";
	const char *l = "E (exclude)";
	const char *m = "x (unknown)";
	const char *n = "o (OS specific)";
	const char *o = "p (processor specific)";

	printf("Key to Flags:\n");
	printf("  %s, %s, %s, %s, %s, %s\n", a, b, c, d, j, k);
	printf("  %s, %s, %s, %s, %s, %s\n", e, f, g, h, l, m);
	printf("  %s, %s, %s\n", i, n, o);
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
