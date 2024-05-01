#include "_getline.h"

static char buffer[READ_SIZE + 1];
static char *buf_pos = buffer;
static int bytes_remaining = 0; /* Renamed from bytes_read to clarify its purpose */
static int total_read = 0;
static int end_of_file_reached = 0;

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
 * find_newline - Find the newline character in a string
 * @start: Pointer to the start of the string to search
 * @size: Size of the string to search
 * Return: Pointer to the first occurrence of the newline character in the
 * string if found, otherwise NULL.
 */
char *find_newline(const char *start, int size) {
    /* Declarations */
    int i;

    /* Logic */
    for (i = 0; i < size; i++) {
        if (start[i] == '\n') {
            return (char *)(start + i);
        }
    }
    return NULL;
}


/**
 * read_line - Read a line from the buffer
 *
 * Return: Pointer to the read line. Memory is allocated for the line, and
 * the caller is responsible for freeing it. Returns NULL if the end of the
 * input is reached or if an error occurs.
 */
char *read_line() {
    /* Declarations */
    char *line = NULL;
    int line_size = 0;
    int newline_found = 0;
    char *newline_pos;

    /* Logic */
    while (!newline_found) {
        if (buf_pos - buffer >= bytes_remaining) {
            /* Buffer is empty, need to fill it */
            int result = fill_buffer(0); /* Read from stdin */
            if (result <= 0) {
                return NULL; /* Error or end of file */
            }
        }

        /* Find the position of the newline character manually */
        newline_pos = find_newline(buf_pos, bytes_remaining);
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
            newline_found = 1;
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
    int result;
    char *line;

    if (fd == -1) {
        reset_buffer();
        return NULL;
    }

    if (end_of_file_reached) {
        end_of_file_reached = 0; /* Reset flag for future use */
        line = read_line();
        if (line) {
            /*printf("%s\n", line);*/
            free(line);
        }
        return NULL; /* End of file reached, return NULL */
    }

    if (total_read == 0 || (buf_pos - buffer >= bytes_remaining)) {
        reset_buffer();
        result = fill_buffer(fd);
        if (result <= 0) {
            end_of_file_reached = 1; /* Set flag to indicate end of file */
            return NULL; /* Error or end of file */
        }
    }

    line = read_line();

    return line;
}
