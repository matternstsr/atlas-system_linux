#ifndef _GETLINE_H_
#define _GETLINE_H_

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READ_SIZE 1024

int fill_buffer(const int fd);
char *find_newline(const char *start, int size);
char *read_line();
void reset_buffer();
char *_getline(const int fd);

#endif /* _GETLINE_H_ */
