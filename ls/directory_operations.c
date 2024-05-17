#include "directory_reader.h"
#include <stdio.h> // Included this line to resolve the error for fprintf

/* Initialize directory reader */
int initDirectoryReader(DirectoryReader *reader, const char *path) {
  DIR *dir = opendir(path);
  if (!dir)
      return -1;
  reader->dir = dir;
  reader->path = path;
  reader->current_entry = NULL;
  reader->finished = 0;
  return 0;
}

/* Get next directory entry */
struct dirent *getNextEntry(DirectoryReader *reader) {
  struct dirent *next_entry = readdir(reader->dir);
  if (next_entry)
      reader->current_entry = next_entry;
  else {
      reader->finished = 1;
      reader->current_entry = NULL;
  }
  return next_entry;
}

/* Iterate through directory entries and apply item handler */
int forEachEntry(DirectoryReader *reader, int (*itemHandler)(DirectoryReader *)) {
  int entry_count = 0;
  while (getNextEntry(reader)) {
      if (itemHandler(reader) == -1) {
          /* Handle error */
          fprintf(stderr, "Error handling directory entry\n");
      }
      ++entry_count;
  }
  return entry_count;
}

/* Destroy directory reader */
void destroyDirectoryReader(DirectoryReader *reader) {
  if (reader && reader->dir) {
      closedir(reader->dir);
      memset(reader, 0, sizeof(*reader));
  }
}
