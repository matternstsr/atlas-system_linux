
#ifndef _GETLINE_H_
#define _GETLINE_H_

#include "_getline.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MEMORY_FILL_VALUE ((char)0xFF)
#define malloc(size) my_malloc(size)
#define READ_SIZE 2048

char *_getline(const int fd);
void reset_buffer();
int fill_buffer(const int fd);
char *read_line();

extern int total_lines;

#endif /* _GETLINE_H_ */

