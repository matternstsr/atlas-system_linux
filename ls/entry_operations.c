#include "directory_reader.h"

/* 
 * printEntryName - Print directory entry name without ANSI escape codes.
 * @reader: Pointer to a DirectoryReader structure.
 * Returns: 0 on success, -1 if reader or reader->current_entry is NULL.
 */
int printEntryName(DirectoryReader *reader)
{
	int is_directory;

	if (!reader || !reader->current_entry)
		return (-1);

	/* Skip hidden files (those starting with a dot) */
	if (reader->current_entry->d_name[0] == '.')
		return (0);

	/* Check if the entry is a directory */
	is_directory = (reader->current_entry->d_type == DT_DIR);

	/* Print directories */
	if (is_directory)
		printf("%s\n", reader->current_entry->d_name);
	else
		printf("%s\n", reader->current_entry->d_name);
	return (0);
}

/* 
 * getEntryTypeName - Get directory entry type name.
 * @d_type: The directory entry type.
 * Returns: A string describing the directory entry type.
 */
const char *getEntryTypeName(unsigned char d_type)
{
	switch (d_type)
	{
		case DT_BLK:
			return ("block_device");
		case DT_CHR:
			return ("character_device");
		case DT_DIR:
				return ("directory");
		case DT_FIFO:
			return ("named_pipe");
		case DT_LNK:
			return ("symbolic_link");
		case DT_REG:
			return ("file");
		case DT_SOCK:
			return ("socket");
		default: /* DT_UNKNOWN */
			return ("unknown");
	}
}
