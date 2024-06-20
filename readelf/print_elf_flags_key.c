#include "1-hreadelf.h"

void print_elf_flags_key(void)
{
    printf("Key to Flags:\n");
    printf("  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)\n");
    printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
    printf("  O (extra OS processing required), o (OS specific), p (processor specific) \n");
}

void print_flag_key(void)
{
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
	printf("  I (info), L (link order), G (group), ");
	printf("T (TLS), E (exclude), x (unknown)\n");
	printf("  O (extra OS processing required) ");
	printf("o (OS specific), p (processor specific)\n");
}