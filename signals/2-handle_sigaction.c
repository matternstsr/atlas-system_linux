#include "signals.h"

/**
 * sigint_handler - Signal handler for SIGINT
 * @signum: Signal number received
 *
 * This function handles the SIGINT (Ctrl-C) signal by printing
 * "Gotcha! [<signum>]" to standard output and flushing the output buffer.
 */
void sigint_handler(int signum)
{
	printf("Gotcha! [%d]\n", signum);
	fflush(stdout);

}

/**
 * handle_sigaction - Set up signal handler for SIGINT using sigaction
 *
 * Return: 0 on success, -1 on error
 *
 * This function sets up a signal handler for the SIGINT signal (Ctrl-C)
 * using the sigaction() function. It installs `sigint_handler` as the
 * handler function for SIGINT signals.
 */
int handle_sigaction(void)
{
	struct sigaction siggy;

	/* Set up the sigaction struct */
	siggy.sa_handler = sigint_handler;
	sigemptyset(&siggy.sa_mask);
	siggy.sa_flags = 0;

	/* Install the SIGINT handler */
	if (sigaction(SIGINT, &siggy, NULL) == -1)
		/* If an error occurs while setting the handler */
		return (-1);

	/* Return 0 on success */
	return (0);
}
