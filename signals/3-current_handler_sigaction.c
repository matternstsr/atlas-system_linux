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
	struct sigaction siggy, old_siggy;

	/* Prepare sigaction struct for SIGINT */
	siggy.sa_handler = SIG_DFL;  /* Set default handler temporarily */
	sigemptyset(&siggy.sa_mask);
	siggy.sa_flags = 0;

	/* Get current handler using sigaction */
	if (sigaction(SIGINT, &siggy, &old_siggy) == -1)
	{
	return (NULL);
	}

	/* Restore original handler */
	sigaction(SIGINT, &old_siggy, NULL);

	/* Return the current handler */
	return (old_siggy.sa_handler);
}
