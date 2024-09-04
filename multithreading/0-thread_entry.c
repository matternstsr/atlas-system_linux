#include <stdio.h>
#include <stdlib.h>
#include "multithreading.h"

/**
* thread_entry - Thread entry point function.
* @arg: arg passed to the thread. (Usually a ptr to data or a str.)
*
* This func is called when the thread starts. It processes the arg
* and does the thread's tasks.
*
* Return: ALWAYS returns NULL.
*/
void *thread_entry(void *arg)
{
	/* Cast argument to a string */
	char *str = (char *)arg;

	/* Print the string */
	printf("%s\n", str);

	/* Explicitly terminate the thread and return NULL */
	/* pthread_exit: This function explicitly terminates the calling thread */
	/* and allows it to return a status value to any joining threads. */
	/* This ensures that the thread’s resources are properly cleaned up and */
	/* that any pthread_join calls made by other threads to wait for this */
	/* thread will receive the exit status. */
	pthread_exit(NULL);
}
