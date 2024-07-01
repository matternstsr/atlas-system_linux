#include "2-hreadelf.h"

/**
 * open_and_handle_file - opens a file descriptor and handles errors
 * @filename: name of the file to open
 * @quiet_mode: if true, suppress error messages
 * Return: file descriptor if successful, -1 on failure
 */
int open_and_handle_file(char *filename, int quiet_mode)
{
    int file_descriptor; // Renamed 'fd' to 'file_descriptor'

    file_descriptor = open(filename, O_RDONLY);
    if (file_descriptor == -1 && !quiet_mode)
    {
        if (errno == EACCES)
            fprintf(stderr, "Error: Access denied for file '%s'\n", filename);
        else if (errno == ENOENT)
            fprintf(stderr, "Error: File '%s' not found\n", filename);
    }
    return file_descriptor;
}
