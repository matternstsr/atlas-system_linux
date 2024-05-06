#ifndef _GETLINE_H_
#define _GETLINE_H_

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MEMORY_FILL_VALUE ((char)0xFF)
#define MY_MALLOC(size) my_malloc(size) /* Renamed the macro */
#define READ_SIZE 2048

void *my_malloc(size_t size); /* Declaration moved before usage */

char *_getline(const int fd);
void reset_buffer(void);
int fill_buffer(const int fd);
char *read_line();

extern int total_lines;

#endif /* _GETLINE_H_  */
