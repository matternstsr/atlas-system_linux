#include "hnm.h"

/**
* crack_open_file - attempts to open a file and returns the file descriptor;
* exits on failure
* @name: name of the file to open
* @silent: if true, do not print error messages
* @argv: the argument vector
* Return: file descriptor (fd) of the opened file
*/
int crack_open_file(char *name, int silent, char **argv)
{
	/* Attempt to open the file specified by 'name' for reading only */
	int fd = open(name, O_RDONLY);

	/* Check if open() failed (fd == -1 indicates failure) */
	if (fd == -1)
	{
		/* If not silent mode is enabled (silent == 0), handle error messages */
		if (!silent)
		{
			/* Flushes any buffered output in stdout */
			fflush(stdout);
			/* Check the error code returned by open() */
			if (errno == EACCES)
			{
				/* Print error message to stderr for permission denied error */
				fprintf(stderr, ERR_NO_ACCESS, argv[0], name);
			}
			else if (errno == ENOENT)
			{
				/* Print error msg to stderr for no such file or dir error */
				fprintf(stderr, ERR_NO_ENTRY, argv[0], name);
			}
		}
		/* Exit the program with a failure status */
		exit(EXIT_FAILURE);
	}
	/* Return the file descriptor (fd) of the opened file */
	return (fd);
}
