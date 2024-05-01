
#ifndef _GETLINE_H_
#define _GETLINE_H_

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define READ_SIZE 1024

char *_getline(const int fd);
void reset_buffer();
int fill_buffer(const int fd);
char *read_line();

extern int total_lines;

#endif /* _GETLINE_H_ */

