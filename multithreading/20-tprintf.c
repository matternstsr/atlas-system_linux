#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

/* Global mutex variable. */
static pthread_mutex_t print_mutex;


/* Constructor function to initialize the mutex */
__attribute__((constructor))



/* Constructor function to initialize the mutex. */
__attribute__((constructor))
static void init_mutex(void)
{
	pthread_mutex_init(&print_mutex, NULL);
}

/* Destructor function to destroy the mutex */
__attribute__((destructor))



/* Destructor function to destroy the mutex. */
__attribute__((destructor))
static void destroy_mutex(void)
{
	pthread_mutex_destroy(&print_mutex);
}

/**
* tprintf - Print a formatted string with the thread ID, using mutex for sync.
* @format: Format string.
*
* Prints the formatted string prefixed by the thread ID. Uses mutex
* for thread-safe output.
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

	/* Lock the mutex */
	pthread_mutex_lock(&print_mutex);

	/* Print the thread ID */
	printf("[%lu] ", (unsigned long)thread_id);

	/* Initialize variable argument list */
	va_start(args, format);

	/* Print the formatted string */
	ret = vprintf(format, args);

	/* End variable argument list */
	va_end(args);

	/* Unlock the mutex */
	pthread_mutex_unlock(&print_mutex);

	/* Return the number of characters printed */
	return (ret);
}
