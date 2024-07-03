#include "2-hreadelf.h"

/**
* check_elf_header - checks if header matches ELF magic bytes
* @EH: buffer holding the ELF header (should be at least 16 bytes)
* Return: 0 if ELF magic bytes match, else 1
*/
int check_elf_header(char *EH)
{
	if (EH[0] == 0x7f && EH[1] == 0x45 &&
		EH[2] == 0x4c && EH[3] == 0x46)
		return (0);/*   ELF magic bytes match */
	else
		return (1); /*  Not ELF */
}
