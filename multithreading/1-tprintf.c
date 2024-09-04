#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "multithreading.h"

/**
* tprintf - Print a formatted string with the thread ID.
* @format: Format string for output.
*
* Prints the formatted string preceded by the thread ID.
*
* Return: Number of characters printed (like `printf`).
*/
int tprintf(const char *format, ...)
{
	va_list args;
	pthread_t thread_id;
	int ret;

	/* Get the current thread ID */
	thread_id = pthread_self();

	/* Print the thread ID */
	printf("[%lu] ", (unsigned long)thread_id);

	/* var arg list */
	va_start(args, format);

	/* Print the formatted string */
	ret = vprintf(format, args);

	/* End var arg list */
	va_end(args);

	/* Return the number of chars printed */
	return (ret);
}
