#include "directory_reader.h"

/**
 * error_message_printing - Prints an error message.
 * @filename: The name of the file or directory causing the error.
 * Return: The error code.
 **/
int error_message_printing(char *filename)
{
	char buffer[256];
	char *error_template;
	int error_code = 2;

	if (errno == 13)
		error_template = "hls: cannot open directory %s";
	else
		error_template = "hls: cannot access %s";
	fprintf(buffer, error_template, filename);
	perror(buffer);
	return error_code;
}
