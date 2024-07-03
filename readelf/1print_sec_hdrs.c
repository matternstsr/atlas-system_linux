#include "1-hreadelf.h"

/**
 * print_32bit_sec_hdrs - Print the 32-bit ELF section headers
 * @elf_header: Pointer to the ELF header structure (32-bit)
 * @section_headers: Pointer to the array of section headers (32-bit)
 * @strtab: Pointer to the string table base address
 */
void print_32bit_sec_hdrs(Elf32_Ehdr *elf_header,
							Elf32_Shdr *section_headers, const char *strtab)
{
	int sec_cnt;
	char *name = NULL;

	printf("There are %d section headers, starting at offset 0x%x:\n",
		elf_header->e_shnum, elf_header->e_shoff);
	printf("\nSection Headers:\n");
	printf("  [Nr] %-17s %-15s %-8s %-6s %-6s %-2s %-3s %-2s %-3s %-2s\n",
		"Name", "Type", "Addr", "Off", "Size",
		"ES", "Flg", "Lk", "Inf", "Al");

	for (sec_cnt = 0; sec_cnt < elf_header->e_shnum; sec_cnt++)
	{
		name = (char *)(strtab + section_headers[sec_cnt].sh_name);
		printf("  [%2d] %-17s %-15s %08x %06x %06x %02x %3s %2u %3u %2u\n",
			sec_cnt,
			name,
			resolve_std_sec_type_name(section_headers[sec_cnt].sh_type),
			section_headers[sec_cnt].sh_addr,
			section_headers[sec_cnt].sh_offset,
			section_headers[sec_cnt].sh_size,
			section_headers[sec_cnt].sh_entsize,
			convert_flags_to_string(section_headers[sec_cnt].sh_flags),
			section_headers[sec_cnt].sh_link,
			section_headers[sec_cnt].sh_info,
			section_headers[sec_cnt].sh_addralign);
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
	int sec_cnt;
	char *name = NULL;

	printf("There are %d section headers, starting at offset 0x%lx:\n",
		elf_header->e_shnum, elf_header->e_shoff);
	printf("\nSection Headers:\n");
	printf("  [Nr] %-17s %-15s %-16s %-6s %-6s %-2s %-3s %-2s %-3s %-2s\n",
		"Name", "Type", "Address", "Off", "Size",
		"ES", "Flg", "Lk", "Inf", "Al");

	for (sec_cnt = 0; sec_cnt < elf_header->e_shnum; sec_cnt++)
	{
		name = (char *)(strtab + section_headers[sec_cnt].sh_name);
		printf("  [%2d] %-17s %-15s %016lx %06lx %06lx %02lx %3s %2u %3u %2lu\n",
			sec_cnt,
			name,
			resolve_std_sec_type_name(section_headers[sec_cnt].sh_type),
			section_headers[sec_cnt].sh_addr,
			section_headers[sec_cnt].sh_offset,
			section_headers[sec_cnt].sh_size,
			section_headers[sec_cnt].sh_entsize,
			convert_flags_to_string(section_headers[sec_cnt].sh_flags),
			section_headers[sec_cnt].sh_link,
			section_headers[sec_cnt].sh_info,
			section_headers[sec_cnt].sh_addralign);
	}
	print_elf_flags_key();/* Assuming this function prints ELF flags key */
	/* 1print_elf_flags_key.c */
}

/**
 * print_32bit_sec_hdrs_be - Print the 32-bit ELF section headers
 * @elf_header: Pointer to the ELF header structure (32-bit)
 * @section_headers: Pointer to the array of section headers (32-bit)
 * @strtab: Pointer to the string table base address
 */
void print_32bit_sec_hdrs_be(Elf32_Ehdr *elf_header,
								Elf32_Shdr *section_headers, const char *strtab)
{
	int sec_cnt;
	char *name = NULL;

	printf("There are %d section headers, starting at offset 0x%x:\n",
		__bswap_16(elf_header->e_shnum), __bswap_32(elf_header->e_shoff));
	printf("\nSection Headers:\n");
	printf("  [Nr] %-17s %-15s %-8s %-6s %-6s %-2s %-3s %-2s %-3s %-2s\n",
		"Name", "Type", "Addr", "Off", "Size",
		"ES", "Flg", "Lk", "Inf", "Al");

	for (sec_cnt = 0; sec_cnt < __bswap_16(elf_header->e_shnum); sec_cnt++)
	{
		name = (char *)(strtab + section_headers[sec_cnt].sh_name);
		printf("  [%2d] %-17s %-15s %08x %06x %06x %02x %3s %2u %3u %2u\n",
			sec_cnt,
			name,
			resolve_std_sec_type_name(section_headers[sec_cnt].sh_type),
			section_headers[sec_cnt].sh_addr,
			section_headers[sec_cnt].sh_offset,
			section_headers[sec_cnt].sh_size,
			section_headers[sec_cnt].sh_entsize,
			convert_flags_to_string(section_headers[sec_cnt].sh_flags),
			section_headers[sec_cnt].sh_link,
			section_headers[sec_cnt].sh_info,
			section_headers[sec_cnt].sh_addralign);
	}
	print_flag_key();/* Assuming this function prints flag keys */
}



