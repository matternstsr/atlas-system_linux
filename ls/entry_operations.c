/* entry_operations.c */

#include "directory_reader.h"

/* Print directory entry name */
int printEntryName(DirectoryReader *reader) {
	int is_directory;

	if (!reader || !reader->current_entry)
			return -1;
	
	/* Check if the entry is a directory */
	is_directory = (reader->current_entry->d_type == DT_DIR);

	/* Print directories in blue */
	if (is_directory) {
			printf("\x1b[34m%s\x1b[0m\n", reader->current_entry->d_name);
	} else {
			printf("%s\n", reader->current_entry->d_name);
	}

	return 0;
}


/* Get directory entry type name */
const char *getEntryTypeName(unsigned char d_type) {
switch (d_type) {
		case DT_BLK:
				return "block_device";
		case DT_CHR:
				return "character_device";
		case DT_DIR:
				return "directory";
		case DT_FIFO:
				return "named_pipe";
		case DT_LNK:
				return "symbolic_link";
		case DT_REG:
				return "file";
		case DT_SOCK:
				return "socket";
		default: /* DT_UNKNOWN */
				return "unknown";
}
}
