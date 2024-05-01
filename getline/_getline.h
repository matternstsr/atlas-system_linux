/* _getline.h */

#ifndef _GETLINE_H_
#define _GETLINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define READ_SIZE 1

char *_getline(const int fd);

#endif /* _GETLINE_H_ */


