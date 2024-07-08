#include "signals.h"

/*
 * Description:
 *   This function is the signal handler for SIGINT (Ctrl-C) signals.
 *   It prints "Gotcha! [<signum>]" to standard output, where <signum>
 *   is the signal number received. It ensures the output is immediately
 *   flushed using fflush(stdout).
 *
 * Parameters:
 *   signum: The signal number received (typically SIGINT).
 *
 * Returns:
 *   None.
 */
/* Function to handle SIGINT signal */
void sigint_handler(int signum)
{
	printf("Gotcha! [%d]\n", signum);
	fflush(stdout);
}

/*
 * Description:
 *   This function sets up a signal handler for the SIGINT signal (Ctrl-C).
 *   It installs `sigint_handler` as the handler function for SIGINT using
 *   the `signal()` function from `<signal.h>`.
 *
 * Parameters:
 *   None.
 *
 * Returns:
 *   0 on success, or -1 if an error occurs while setting the signal handler.
 */
/* Function to set up signal handler for SIGINT */
int handle_signal(void)
{
	/* Set the signal handler */
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		/* If an error occurs while setting the signal handler */
		return (-1);
	/* Return 0 on success */
	return (0);
}
