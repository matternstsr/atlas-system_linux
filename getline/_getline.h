#ifndef _GETLINE_H_
#define _GETLINE_H_

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MEMORY_FILL_VALUE ((char)0xFF)
#define MY_MALLOC(size) my_malloc(size) /* Renamed the macro */
#define READ_SIZE 2048

/**
 * my_malloc - Allocates memory and initializes it to a fill value
 * @size: Size of memory to allocate
 * Return: Pointer to the allocated memory
 */
void *my_malloc(size_t size); /* Declaration moved before usage */

/**
 * _getline - Reads a line from a file descriptor
 * @fd: File descriptor
 * Return: Pointer to the read line or NULL if end of file
 */
char *_getline(const int fd);

/**
 * reset_buffer - Resets the buffer position and bytes remaining
 */
void reset_buffer(void);

/**
 * fill_buffer - Fills the buffer with data from a file descriptor
 * @fd: File descriptor
 * Return: Number of bytes read or error indicator
 */
int fill_buffer(const int fd);

/**
 * read_line - Reads a line from the buffer
 * Return: Pointer to the read line
 */
char *read_line();

extern int total_lines;

#endif /* _GETLINE_H_  */
