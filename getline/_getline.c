#include "_getline_copy.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MEMORY_FILL_VALUE ((char)0xFF)
#define MAX_FDS 10 // Maximum number of file descriptors supported

/* Redefine malloc using a macro */
#define malloc(size) my_malloc(size)

static char buffers[MAX_FDS][READ_SIZE];
static char *buf_pos[MAX_FDS] = {buffers[0]};
static int bytes_remaining[MAX_FDS];
static int end_of_file_reached[MAX_FDS];

/**
 * my_malloc - Allocates memory and initializes it to a fill value
 * @size: Size of memory to allocate
 * Return: Pointer to the allocated memory
 */
void *my_malloc(size_t size)
{
    void *ptr = malloc(size);

    if (ptr != NULL)
    {
        /* Initialize allocated memory to 0xFF */
        memset(ptr, MEMORY_FILL_VALUE, size);
    }
    return ptr;
}

/**
 * reset_buffer - Resets the buffer position and bytes remaining for a given fd
 * @fd: File descriptor
 */
void reset_buffer(int fd)
{
    buf_pos[fd] = buffers[fd];
    bytes_remaining[fd] = 0;
}

/**
 * free_resources - Free resources and reset static variables for all file descriptors
 */
void free_resources(void)
{
    for (int fd = 0; fd < MAX_FDS; fd++)
    {
        reset_buffer(fd);
        end_of_file_reached[fd] = 0;
    }
}

/**
 * _getline - Reads a line from a file descriptor
 * @fd: File descriptor
 * Return: Pointer to the read line or NULL if end of file
 */
char *_getline(const int fd)
{
    /* Counter variable for the number of calls to read */
    static int read_calls;

    if (fd == -1)
    {
        free_resources();
        return NULL;
    }

    if (end_of_file_reached[fd])
    {
        end_of_file_reached[fd] = 0;
        char *line = read_line(fd);
        if (line)
            free(line);
        return NULL;
    }

    if (bytes_remaining[fd] == 0 || (buf_pos[fd] - buffers[fd] >= bytes_remaining[fd]))
    {
        reset_buffer(fd);
        int result = fill_buffer(fd);
        if (result <= 0)
        {
            if (result == 0)
                end_of_file_reached[fd] = 1; /* End of file reached */
            return NULL;
        }
        /* Increment the counter when fill_buffer is called */
        read_calls++;
    }

    char *line = read_line(fd);
    return line;
}

/**
 * fill_buffer - Fills the buffer with data from a file descriptor
 * @fd: File descriptor
 * Return: Number of bytes read or error indicator
 */
int fill_buffer(const int fd)
{
    int result;

    result = read(fd, buffers[fd], READ_SIZE);
    if (result <= 0)
    {
        return result; /* Error or end of file */
    }
    buf_pos[fd] = buffers[fd];
    bytes_remaining[fd] = result;
    return result;
}

/**
 * find_newline - Finds the position of the first newline character in a buffer
 * @start: Pointer to the start of the buffer
 * @size: Size of the buffer
 * Return: Pointer to the newline character or NULL if not found
 */
char *find_newline(const char *start, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        if (start[i] == '\n')
        {
            return ((char *)(start + i));
        }
    }
    return NULL;
}

/**
 * read_line - Reads a line from the buffer
 * Return: Pointer to the read line
 */
char *read_line(int fd)
{
    int line_size = 0, newline_found = 0, line_length, remaining_size;
    char *line = NULL, *newline_pos;

    while (!newline_found)
    {
        if (buf_pos[fd] - buffers[fd] >= bytes_remaining[fd])
        {
            int result = fill_buffer(fd); /* Read from file descriptor */
            if (result <= 0)
            {
                if (line_size == 0)
                    return NULL; /* No more lines and buffer is empty */
                newline_found = 1; /* End of file reached */
                break;
            }
        }
        newline_pos = find_newline(buf_pos[fd], bytes_remaining[fd]);
        if (newline_pos != NULL)
        {
            line_length = newline_pos - buf_pos[fd];
            line = realloc(line, line_size + line_length + 1);
            if (!line)
                return NULL; /* Memory allocation failed */
            memcpy(line + line_size, buf_pos[fd], line_length);
            line_size += line_length, line[line_size] = '\0', buf_pos[fd] = newline_pos + 1;
            newline_found = 1;
        }
        else
        {
            remaining_size = bytes_remaining[fd] - (buf_pos[fd] - buffers[fd]);
            line = realloc(line, line_size + remaining_size + 1);
            if (!line)
                return NULL; /* Memory allocation failed */
            memcpy(line + line_size, buf_pos[fd], remaining_size);
            line_size += remaining_size, buf_pos[fd] += remaining_size;
            line[line_size] = '\0';
        }
    }
    return line;
}
