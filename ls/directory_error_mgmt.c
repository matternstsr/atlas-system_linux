/* atlas-system_linux/ls/directory_error_mgmt.c */

#include "directory_reader.h"

/**
 * error_message_printing - Prints an error message.
 * @filename: The name of the file or directory causing the error.
 * Return: The error code.
 **/
int error_message_printing(char *filename)
{
	/* Declare buffer and error_template variables */
	char buffer[256];
	char *error_template;
	int error_code = 2;

	/* Check errno for specific error code */
	if (errno == 13)
		error_template = "hls: cannot open directory %s";
	else
		error_template = "hls: cannot access %s";
	/* Format error message using sprintf */
	sprintf(buffer, error_template, filename);
	/* Print error message with perror */
	perror(buffer);
	/* Return the error code */
	return error_code;
}
