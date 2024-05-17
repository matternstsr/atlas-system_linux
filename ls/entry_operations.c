#include "directory_reader.h"
#include <stdio.h>

/* Print directory entry name */
int printEntryName(DirectoryReader *reader) {
  if (!reader || !reader->current_entry)
      return -1;
  if (reader->current_entry->d_name[0] != '.' || reader->show_all)
      printf("%s\n", reader->current_entry->d_name);
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
