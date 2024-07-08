#include "signals.h"

/* Signal handler for SIGINT */
int sigint_handler(int signum)
{
	printf("Caught %d\nSignal received\n", signum);
	fflush(stdout);
	signal(SIGINT, SIG_DFL); /* Restore default handler */
	raise(SIGINT); /* Raise SIGINT again to exit */
}

/* Function to set up signal handler for SIGINT and handle it once */
int handle_sigint_and_exit(int)
{
	struct sigaction siggy;
	siggy.sa_handler = sigint_handler;
	sigemptyset(&siggy.sa_mask);
	siggy.sa_flags = 0;

	return (sigaction(SIGINT, &siggy, NULL));
}

int main() {
	handle_sigint_and_exit(); /* Call func to setup handler & handle SIGINT */
	return (EXIT_SUCCESS);
}
