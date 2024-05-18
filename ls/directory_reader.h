#ifndef DIRECTORY_READER_H
#define DIRECTORY_READER_H

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <strings.h>
#include <ctype.h> /* Conv chars to lower for comp */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define INITIAL_CAPACITY 100

/**
 * struct dirent - Directory entry structure
 *
 * Description: This structure represents a directory entry,
 * containing information about a file or directory.
 */
typedef struct DirectoryReader
{
	DIR *dir;
	const char *path;
	struct dirent *current_entry;
	int finished;
	int show_all;
} DirectoryReader;

/* Function prototypes */
int initDirectoryReader(DirectoryReader *reader, const char *path);
struct dirent *getNextEntry(DirectoryReader *reader);
int forEachEntry(DirectoryReader *reader,
                 int (*itemHandler)(DirectoryReader *));
void destroyDirectoryReader(DirectoryReader *reader);
const char *getEntryTypeName(unsigned char d_type);
const char *joinPath(const char *dirpath, const char *entry_name);
int printEntryName(DirectoryReader *reader);
int mattcomp(const char *str1, const char *str2);
void *mattset(void *ptr, int value, size_t num);
void mattsort(struct dirent **entries, int num_entries);
char mattlower(char c);
int checkEmpty(DirectoryReader *reader);
const char *mattError(int errnum);
int countEntries(const char *name, const struct stat *status, int type,
                 void *arg);

#endif /* DIRECTORY_READER_H */
