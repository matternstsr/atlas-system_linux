#include "1-hreadelf.h"

const char *convert_flags_to_string(uint64_t flags)
{
static char flag_string[16];
int position = 0;

if (flags & SHF_WRITE)
	flag_string[position++] = 'W';
if (flags & SHF_ALLOC)
	flag_string[position++] = 'A';
if (flags & SHF_EXECINSTR)
	flag_string[position++] = 'X';
if (flags & SHF_MERGE)
	flag_string[position++] = 'M';
if (flags & SHF_STRINGS)
	flag_string[position++] = 'S';
if (flags & SHF_INFO_LINK)
	flag_string[position++] = 'I';
if (flags & SHF_LINK_ORDER)
	flag_string[position++] = 'L';
if (flags & SHF_GROUP)
	flag_string[position++] = 'G';
if (flags & SHF_TLS)
	flag_string[position++] = 'T';
if (flags & SHF_EXCLUDE)
{
	flag_string[position++] = 'E';
	flag_string[position] = '\0';
	return flag_string;
}
if (flags & SHF_MASKOS)
	flag_string[position++] = 'o';
if (flags & SHF_MASKPROC)
	flag_string[position++] = 'p';
if (flags & SHF_OS_NONCONFORMING)
	flag_string[position++] = 'O';
flag_string[position] = '\0';
if (position == 0)
	return " ";
return flag_string;
}