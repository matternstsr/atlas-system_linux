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

	/* Return NULL */
	pthread_exit(NULL);
}
