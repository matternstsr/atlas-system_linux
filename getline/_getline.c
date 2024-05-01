#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "_getline.h"

#define READ_SIZE 1024

static char buffer[READ_SIZE + 1];
static char *buf_pos = buffer;
static int bytes_remaining = 0; /* Renamed from bytes_read to clarify its purpose */
static int total_read = 0;

/**
 * fill_buffer - Fill the buffer with data from a file descriptor
 * @fd: File descriptor to read from
 *
 * Return: Number of bytes read or error indicator
 */
int fill_buffer(const int fd) {
    int result = read(fd, buffer, READ_SIZE);
    if (result <= 0) {
        return result; /* Error or end of file */
    }
    buf_pos = buffer;
    bytes_remaining = result; /* Renamed from bytes_read to clarify its purpose */
    total_read += result;
    return result;
}

/**
 * read_line - Read a line from the buffer
 *
 * Return: Pointer to the read line
 */
char *read_line() {
    char *line = NULL;
    int line_size = 0;
    int newline_found = 0;
    char *newline_pos;

    while (!newline_found) {
        if (buf_pos - buffer >= bytes_remaining) {
            /* Buffer is empty, need to fill it */
            int result = fill_buffer(0); /* Read from stdin */
            if (result <= 0) {
                return NULL; /* Error or end of file */
            }
        }

        /* Find the position of the newline character manually */
        newline_pos = strchr(buf_pos, '\n');
        if (newline_pos != NULL) {
            /* Newline character found */
            int line_length = newline_pos - buf_pos;
            line = realloc(line, line_size + line_length + 1);
            if (!line) {
                return NULL; /* Memory allocation failed */
            }
            memcpy(line + line_size, buf_pos, line_length);
            line_size += line_length;
            line[line_size] = '\0';
            buf_pos = newline_pos + 1; /* Move buffer position after newline character */
            newline_found = 1;
        } else {
            /* Newline character not found in the current buffer */
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

/**
 * reset_buffer - Reset the buffer
 */
void reset_buffer() {
    buf_pos = buffer;
    bytes_remaining = 0;
    total_read = 0;
}

/**
 * _getline - Get the next line from a file descriptor
 * @fd: File descriptor to read from
 *
 * Return: Pointer to the read line
 */
char *_getline(const int fd) {
    if (fd == -1) {
        reset_buffer();
        return NULL;
    }

    /* If total_read is non-zero, the buffer already contains some data from a previous read */
    if (total_read == 0 || (buf_pos - buffer >= bytes_remaining)) {
        /* Reset buffer position when reaching the end or at the beginning */
        reset_buffer();
        int result = fill_buffer(fd);
        if (result <= 0) {
            return NULL; /* Error or end of file */
        }
    }

    return read_line();
}
