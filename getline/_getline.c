#include "_getline.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MEMORY_FILL_VALUE ((char)0xFF)

/* Redefine malloc using a macro */
#define malloc(size) my_malloc(size)

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
return (ptr);
}

static char buffer[READ_SIZE];
static char *buf_pos = buffer;
static int bytes_remaining;
static int end_of_file_reached;

/**
 * fill_buffer - Fills the buffer with data from a file descriptor
 * @fd: File descriptor
 * Return: Number of bytes read or error indicator
 */
int fill_buffer(const int fd)
{
int result;

result = read(fd, buffer, READ_SIZE);
if (result <= 0)
{
	return (result); /* Error or end of file */
}
buf_pos = buffer;
bytes_remaining = result;
return (result);
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
return (NULL);
}

/**
 * read_line - Reads a line from a file descriptor
 * @fd: File descriptor
 * Return: Pointer to the read line or NULL if end of file
 */
char *read_line(const int fd)
{ /* Function body remains the same */
int result, line_size = 0, newline_found = 0, line_length, remaining_size;
char *line = NULL, *newline_pos;

while (!newline_found)
{
	if (buf_pos - buffer >= bytes_remaining)
	{
		result = fill_buffer(fd); /* Read from stdin */
		if (result <= 0)
		{
			if (line_size == 0)
				return (NULL); /* No more lines and buffer is empty */
			newline_found = 1; /* End of file reached */
			break;
		}
	}
	newline_pos = find_newline(buf_pos, bytes_remaining);
	if (newline_pos != NULL)
	{
		line_length = newline_pos - buf_pos;
		line = realloc(line, line_size + line_length + 1);
		if (!line)
			return (NULL); /* Memory allocation failed */
		memcpy(line + line_size, buf_pos, line_length);
		line_size += line_length, line[line_size] = '\0', buf_pos = newline_pos + 1;
		newline_found = 1;
	}
	else
	{
		remaining_size = bytes_remaining - (buf_pos - buffer);
		line = realloc(line, line_size + remaining_size + 1);
		if (!line)
			return (NULL); /* Memory allocation failed */
		memcpy(line + line_size, buf_pos, remaining_size);
		line_size += remaining_size, buf_pos += remaining_size;
		line[line_size] = '\0';
	}
}
return (line);
}

/**
 * reset_buffer - Resets the buffer position and bytes remaining
 */
void reset_buffer(void)
{
buf_pos = buffer;
bytes_remaining = 0;
}

/**
 * _getline - Reads a line from a file descriptor
 * @fd: File descriptor
 * Return: Pointer to the read line or NULL if end of file
 */
char *_getline(const int fd)
{
char *line;
int result;

/* Counter variable for the number of calls to read */
static int read_calls;
if (fd == -1)
{
	reset_buffer();
	return (NULL);
}
if (end_of_file_reached)
{
	end_of_file_reached = 0;
	line = read_line(fd);
	if (line)
		free(line);
	return (NULL);
}
if (bytes_remaining == 0 || (buf_pos - buffer >= bytes_remaining))
{
	reset_buffer();
	result = fill_buffer(fd);
	if (result <= 0)
	{
		if (result == 0)
			end_of_file_reached = 1; /* End of file reached */
		return (NULL);
	} /* Increment the counter when fill_buffer is called */
	read_calls++;
}
line = read_line(fd);
return (line);
}
