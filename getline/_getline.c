#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include "_getline.h"

#define READ_SIZE 1024

static char *buffer = NULL;
static int buffer_index = 0;
static int buffer_size = 0;
int start;
int end;
int line_length;
char *line;
int i;


char *_getline(const int fd) {
    if (fd == -1) { 
        free(buffer);
        buffer = NULL;
        buffer_index = 0;
        buffer_size = 0;
        return NULL;
    }

    if (buffer == NULL || buffer_index >= buffer_size) {
        buffer_size = read(fd, buffer, READ_SIZE);
        if (buffer_size <= 0) {
            free(buffer);
            buffer = NULL;
            return NULL;
        }
        buffer_index = 0;
    }

    start = buffer_index;
    while (buffer_index < buffer_size && buffer[buffer_index] != '\n') {
        buffer_index++;
    }

    end = buffer_index;
    if (buffer_index < buffer_size && buffer[buffer_index] == '\n') {
        buffer_index++;
    }

    line_length = end - start;
    if (line_length == 0) {
        return NULL;
    }

    line = (char *)malloc((line_length + 1) * sizeof(char));
    if (line == NULL) {
        return NULL;
    }

    for (i = 0; i < line_length; i++) {
        line[i] = buffer[start + i];
    }
    line[line_length] = '\0';

    return line;
}
