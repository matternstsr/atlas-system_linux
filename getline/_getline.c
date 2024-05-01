#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "_getline.h"

#define READ_SIZE 1024

static char buffer[READ_SIZE + 1];
static char *buf_pos;
static int bytes_remaining;
static int total_read;

int fill_buffer(const int fd) {
    int result;
    result = read(fd, buffer, READ_SIZE);
    if (result <= 0) {
        return result; /* Error or end of file */
    }
    buf_pos = buffer;
    bytes_remaining = result;
    total_read += result;
    return result;
}

char *read_line() {
    char *line = NULL;
    int line_size = 0;
    int newline_found = 0;
    char *newline_pos;

    while (!newline_found) {
        if (buf_pos - buffer >= bytes_remaining) {
            int result = fill_buffer(0); /* Read from stdin */
            if (result <= 0) {
                return NULL; /* Error or end of file */
            }
        }

        newline_pos = strchr(buf_pos, '\n');
        if (newline_pos != NULL) {
            int line_length = newline_pos - buf_pos;
            line = realloc(line, line_size + line_length + 1);
            if (!line) {
                return NULL; /* Memory allocation failed */
            }
            memcpy(line + line_size, buf_pos, line_length);
            line_size += line_length;
            line[line_size] = '\0';
            buf_pos = newline_pos + 1;
            newline_found = 1;
        } else {
            int remaining_size = bytes_remaining - (buf_pos - buffer);
            line = realloc(line, line_size + remaining_size + 1);
            if (!line) {
                return NULL; /* Memory allocation failed */
            }
            memcpy(line + line_size, buf_pos, remaining_size);
            line_size += remaining_size;
            buf_pos += remaining_size;
            line[line_size] = '\0';
        }
    }

    return line;
}

void reset_buffer() {
    buf_pos = buffer;
    bytes_remaining = 0;
    total_read = 0;
}

char *_getline(const int fd) {
    if (fd == -1) {
        reset_buffer();
        return NULL;
    }

    if (total_read == 0 || (buf_pos - buffer >= bytes_remaining)) {
        reset_buffer();
        int result = fill_buffer(fd);
        if (result <= 0) {
            return NULL; /* Error or end of file */
        }
    }

    return read_line();
}
