#ifndef DIRECTORY_READER_H
#define DIRECTORY_READER_H

#include <dirent.h>

/* Structure to handle directory reading */
typedef struct DirectoryReader {
    DIR *dir;
    const char *path;
    struct dirent *current_entry;
    int finished;
    int show_all;
} DirectoryReader;

/* Function prototypes */
int initDirectoryReader(DirectoryReader *reader, const char *path);
struct dirent *getNextEntry(DirectoryReader *reader);
int forEachEntry(DirectoryReader *reader, int (*itemHandler)(DirectoryReader *));
void destroyDirectoryReader(DirectoryReader *reader);
const char *getEntryTypeName(unsigned char d_type);
const char *joinPath(const char *dirpath, const char *entry_name);
int printEntryName(DirectoryReader *reader);

#endif /* DIRECTORY_READER_H */
