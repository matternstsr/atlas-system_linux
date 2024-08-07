#include <stdio.h>
#include <signal.h>

/**
 * current_handler_sigaction - Retrieve current handler
 * for SIGINT using sigaction
 *
 * This function retrieves the current handler for the SIGINT signal (Ctrl-C)
 * using the sigaction() function from <signal.h>. It temporarily sets the
 * signal handler to SIG_DFL (default handler), retrieves the current handler,
 * and restores the original handler. It returns a pointer to the current
 * SIGINT signal handler function.
 *
 * Return:
 *   Pointer to the current SIGINT signal handler function, or NULL on failure.
 */
void (*current_handler_sigaction(void))(int)
{
	struct sigaction siggy;
	/* Get current handler using sigaction */
	if (sigaction(SIGINT, NULL, &siggy) == -1)
	{
	return (NULL);
	}

	/* Return the current handler */
	return (siggy.sa_handler);
}
