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
 * handle_signal - Set up signal handler for SIGINT
 * 
 * Return: 0 on success, -1 on failure
 * 
 * This function installs the sigint_handler function as the handler
 * for SIGINT signals using the signal() function from <signal.h>.
 * It returns 0 on success or -1 if an error occurs while setting
 * the signal handler.
 */
int handle_signal(void)
{
	/* Set the signal handler */
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		/* If an error occurs while setting the signal handler */
		return (-1);
	/* Return 0 on success */
	return (0);
}
