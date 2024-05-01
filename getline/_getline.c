// _getline.c

#include "_getline.h"

#define MAX_FILES 1024
#define BUFFER_SIZE 1024

static char *buffers[MAX_FILES] = {0};
static ssize_t buffer_sizes[MAX_FILES] = {0};
static size_t buffer_positions[MAX_FILES] = {0};

char *_getline(const int fd) {
    if (fd == -1) {
        // Free all allocated buffers
        for (int i = 0; i < MAX_FILES; i++) {
            free(buffers[i]);
            buffers[i] = NULL;
            buffer_sizes[i] = 0;
            buffer_positions[i] = 0;
        }
        return NULL;
    }

    if (fd >= MAX_FILES || fd < 0) {
        return NULL; // File descriptor out of range
    }

    if (!buffers[fd]) {
        // Allocate buffer for this file descriptor
        buffers[fd] = (char *)malloc(BUFFER_SIZE);
        if (!buffers[fd]) {
            return NULL; // Error allocating memory
        }
        ssize_t bytes_read = read(fd, buffers[fd], BUFFER_SIZE);
        if (bytes_read <= 0) {
            free(buffers[fd]);
            buffers[fd] = NULL;
            return NULL; // Error reading file or end of file
        }
        buffer_sizes[fd] = bytes_read;
        buffer_positions[fd] = 0;
    }

    // Find the next line in the buffer
    char *line = NULL;
    for (size_t i = buffer_positions[fd]; i < (size_t)buffer_sizes[fd]; i++) {
        if (buffers[fd][i] == '\n') {
            size_t line_length = i - buffer_positions[fd] + 1;
            line = (char *)malloc(line_length);
            if (!line) {
                return NULL; // Error allocating memory
            }
            memcpy(line, buffers[fd] + buffer_positions[fd], line_length - 1);
            line[line_length - 1] = '\0';
            buffer_positions[fd] = i + 1;
            break;
        }
    }

    return line;
}
