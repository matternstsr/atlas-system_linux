#include <unistd.h>
#include <stdlib.h>
#include "_getline.h"

char *_getline(const int fd)
{
    static char buffer[READ_SIZE + 1];
    static char *buf_pos = buffer;
    static int bytes_read = 0;
    static int total_read = 0;

    if (fd == -1) {
        buf_pos = buffer;
        bytes_read = 0;
        total_read = 0;
        return NULL;
    }

    char *line = NULL;
    int line_size = 0;
    int newline_found = 0;

    while (!newline_found) {
        if (buf_pos - buffer >= bytes_read) {
            // fill up buffer if it's empty
            bytes_read = read(fd, buffer, READ_SIZE);
            if (bytes_read <= 0) {
                // No more datas or errors
                return NULL; // error handle
            }
            buf_pos = buffer;
            total_read += bytes_read;
        }

        // Search for newline character
        char *newline_pos = buf_pos;
        while (newline_pos < buffer + bytes_read && *newline_pos != '\n') {
            newline_pos++;
        }

        if (newline_pos < buffer + bytes_read) {
            // IF found newline character
            int line_length = newline_pos - buf_pos;
            line = realloc(line, line_size + line_length + 1);
            if (!line) {
                return NULL; // error handle
            }
            memcpy(line + line_size, buf_pos, line_length);
            line_size += line_length;
            line[line_size] = '\0';
            buf_pos = newline_pos + 1; // Move buffer position after newline character
            newline_found = 1;
        } else {
            // Newline character not found in the current buffer
            int remaining_size = buffer + bytes_read - buf_pos;
            line = realloc(line, line_size + remaining_size + 1);
            if (!line) {
                return NULL; // error handle
            }
            memcpy(line + line_size, buf_pos, remaining_size);
            line_size += remaining_size;
            buf_pos += remaining_size;
            line[line_size] = '\0';
        }
    }

    return line;
}
