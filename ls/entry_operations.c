/* entry_operations.c */

#include "directory_reader.h"

/* Print directory entry name */
int printEntryName(DirectoryReader *reader) {
	if (!reader || !reader->current_entry)
			return -1;
	if (reader->current_entry->d_name[0] != '.') {
			if (reader->current_entry->d_type == DT_DIR) {
					printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "  ", reader->current_entry->d_name);
			} else {
					printf("%s  ", reader->current_entry->d_name);
			}
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
