#include "1-hreadelf.h"
#include <stdio.h>
#include <string.h>

/**
 * print_32bit_sec_hdrs - Print the 32-bit ELF section headers
 * @elf_header: Pointer to the ELF header structure (32-bit)
 * @section_headers: Pointer to the array of section headers (32-bit)
 * @strtab: Pointer to the string table base address
 */
void print_32bit_sec_hdrs(Elf32_Ehdr *elf_header,
													Elf32_Shdr *section_headers, const char *strtab)
{
	int section_count;
	char *name = NULL;

	printf("There are %d section headers, starting at offset 0x%x:\n",
		elf_header->e_shnum, elf_header->e_shoff);
	printf("\nSection Headers:\n");
	printf("  [Nr] %-17s %-15s %-8s %-6s %-6s %-2s %-3s %-2s %-3s %-2s\n",
		"Name", "Type", "Addr", "Off", "Size",
		"ES", "Flg", "Lk", "Inf", "Al");

	for (section_count = 0; section_count < elf_header->e_shnum; section_count++)
	{
		name = (char *)(strtab + section_headers[section_count].sh_name);
		printf("  [%2d] %-17s %-15s %08x %06x %06x %02x %3s %2u %3u %2u\n",
			section_count,
			name,
			resolve_std_sec_type_name(section_headers[section_count].sh_type),
			section_headers[section_count].sh_addr,
			section_headers[section_count].sh_offset,
			section_headers[section_count].sh_size,
			section_headers[section_count].sh_entsize,
			convert_flags_to_string(section_headers[section_count].sh_flags),
			section_headers[section_count].sh_link,
			section_headers[section_count].sh_info,
			section_headers[section_count].sh_addralign);
		if (strcmp(name, ".SUNW_version") == 0) {
            handle_sunw_version_section(section_headers + section_count, strtab);
        } else if (strcmp(name, ".SUNW_versym") == 0) {
            handle_sunw_versym_section(section_headers + section_count, strtab);
        }
	}
	print_flag_key();/* Assuming this function prints flag keys */
}

#include "1-hreadelf.h"

/**
 * print_64bit_sec_headers - Print the 64-bit ELF section headers
 * @elf_header: Pointer to the ELF header structure (64-bit)
 * @section_headers: Pointer to the array of section headers (64-bit)
 * @strtab: Pointer to the string table base address
 */
void print_64bit_sec_headers(Elf64_Ehdr *elf_header,
														Elf64_Shdr *section_headers, const char *strtab)
{
	int section_count;
	char *name = NULL;

	printf("There are %d section headers, starting at offset 0x%lx:\n",
		elf_header->e_shnum, elf_header->e_shoff);
	printf("\nSection Headers:\n");
	printf("  [Nr] %-17s %-15s %-16s %-6s %-6s %-2s %-3s %-2s %-3s %-2s\n",
		"Name", "Type", "Address", "Off", "Size",
		"ES", "Flg", "Lk", "Inf", "Al");

	for (section_count = 0; section_count < elf_header->e_shnum; section_count++)
	{
		name = (char *)(strtab + section_headers[section_count].sh_name);
		printf("  [%2d] %-17s %-15s %016lx %06lx %06lx %02lx %3s %2u %3u %2lu\n",
			section_count,
			name,
			resolve_std_sec_type_name(section_headers[section_count].sh_type),
			section_headers[section_count].sh_addr,
			section_headers[section_count].sh_offset,
			section_headers[section_count].sh_size,
			section_headers[section_count].sh_entsize,
			convert_flags_to_string(section_headers[section_count].sh_flags),
			section_headers[section_count].sh_link,
			section_headers[section_count].sh_info,
			section_headers[section_count].sh_addralign);
		if (strcmp(name, ".SUNW_version") == 0) {
            handle_sunw_version_section(section_headers + section_count, strtab);
        } else if (strcmp(name, ".SUNW_versym") == 0) {
            handle_sunw_versym_section(section_headers + section_count, strtab);
        }
	}
	print_elf_flags_key();/* Assuming this function prints ELF flags key */
	/* 1print_elf_flags_key.c */
}

void handle_sunw_version_section(Elf64_Shdr *section_header, const char *strtab)
{
    printf("    Special handling for .SUNW_version\n");
    // Implement your logic for processing .SUNW_version section
    // Example: Print each entry in the .SUNW_version section
    int entry_size = sizeof(Elf64_Versym); // Adjust for 32-bit if necessary
    int num_entries = section_header->sh_size / entry_size;
    Elf64_Versym *versyms = (Elf64_Versym *)((char *)section_header + section_header->sh_offset);

    printf("    .SUNW_version entries:\n");
    for (int i = 0; i < num_entries; i++) {
        printf("        Entry %d: Version Index = %d\n", i, versyms[i].vs_index);
    }
}

void handle_sunw_versym_section(Elf64_Shdr *section_header, const char *strtab)
{
    printf("    Special handling for .SUNW_versym\n");
    // Implement your logic for processing .SUNW_versym section
    // Example: Print each entry in the .SUNW_versym section
    int entry_size = sizeof(Elf64_Versym); // Adjust for 32-bit if necessary
    int num_entries = section_header->sh_size / entry_size;
    Elf64_Versym *versyms = (Elf64_Versym *)((char *)section_header + section_header->sh_offset);

    printf("    .SUNW_versym entries:\n");
    for (int i = 0; i < num_entries; i++) {
        printf("        Entry %d: Version Index = %d\n", i, versyms[i].vs_index);
    }
}