/* directory_reader.h */

#ifndef DIRECTORY_READER_H
#define DIRECTORY_READER_H

#include <dirent.h>
#include <stdio.h> /* Included this line to resolve the error for fprintf and memeset*/
#include <string.h> /* Include this line to resolve the error of memeset */
#include <stdlib.h> /* Included this line to resolve the undeclared symbols and for qsort */
#include <stddef.h> /* Included this line to resolve issues for my_functions */
#include <strings.h> /* Include strings.h for strcasecmp */

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
int mattcomp(const char *str1, const char *str2);
void *mattset(void *ptr, int value, size_t num);


#endif /* DIRECTORY_READER_H */