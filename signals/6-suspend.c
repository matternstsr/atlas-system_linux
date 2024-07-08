#include "signals.h"

/**
 * sigint_handler - Signal handler for SIGINT
 * @signum: Signal number received (should be SIGINT)
 * 
 * This function handles the SIGINT signal (Ctrl-C) by printing "Caught %d" where
 * %d is the signal number received, followed by "Signal received". It then restores
 * the default handler for SIGINT and raises SIGINT again to exit the program.
 */
void sigint_handler(int signum)
{
	printf("Caught %d\nSignal received\n", signum);
	fflush(stdout);
	signal(SIGINT, SIG_DFL); /* Restore default handler */
	raise(SIGINT); /* Raise SIGINT again to exit */
}

/**
 * handle_sigint_and_exit - Set up signal handler for SIGINT and handle it once
 * 
 * This function sets up a signal handler for SIGINT using sigaction and handles
 * the SIGINT signal once by calling sigint_handler. After handling SIGINT, it
 * suspends the program until another signal is received.
 */
void handle_sigint_and_exit(void)
{
	struct sigaction siggy;

	siggy.sa_handler = sigint_handler;
	sigemptyset(&siggy.sa_mask);
	siggy.sa_flags = 0;

	sigaction(SIGINT, &siggy, NULL);

	pause();
}

int main(void)
{
	handle_sigint_and_exit(); /* Call func to setup handler & handle SIGINT */
	return (EXIT_SUCCESS);
}
