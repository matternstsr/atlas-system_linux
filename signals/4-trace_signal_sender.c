#include "signals.h"

/* Signal handler for SIGQUIT */
void sigquit_handler(int signum, siginfo_t *info, void *context)
{
	void context;
	if (signum == SIGQUIT)
	{	
		/* Print the message */
		printf("SIGQUIT sent by %d\n", info->si_pid);
	}
}

/* Function to set up signal handler for SIGQUIT */
int trace_signal_sender(void)
{
	struct sigaction siggy;

	/* Set up the sigaction struct */
	siggy.sa_sigaction = sigquit_handler;
	sigemptyset(&siggy.sa_mask);
	siggy.sa_flags = SA_SIGINFO;

	/* Install the SIGQUIT handler */
	return (sigaction(SIGQUIT, &siggy, NULL))
	return (0);
}
