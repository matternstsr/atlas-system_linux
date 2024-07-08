#include "signals.h"

/**
 * sigquit_handler - Signal handler for SIGQUIT
 * @signum: Signal number received
 * @info: Pointer to siginfo_t structure containing details about the signal
 * @context: Pointer to context information (unused in this handler)
 *
 * This function handles the SIGQUIT signal (Control-\ in a shell) by printing
 * "SIGQUIT sent by <pid>" to standard output, where <pid> is the PID of the
 * process that sent the signal.
 */
void sigquit_handler(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (signum == SIGQUIT)
	{
		/* Print the message */
		printf("SIGQUIT sent by %d\n", info->si_pid);
	}
}

/**
 * sigquit_handler - Signal handler for SIGQUIT
 * @signum: Signal number received
 * @info: Pointer to siginfo_t structure containing details about the signal
 * @context: Pointer to context information (unused in this handler)
 *
 * This function handles the SIGQUIT signal (Control-\ in a shell) by printing
 * "SIGQUIT sent by <pid>" to standard output, where <pid> is the PID of the
 * process that sent the signal.
 */
int trace_signal_sender(void)
{
	struct sigaction siggy;

	/* Set up the sigaction struct */
	siggy.sa_sigaction = sigquit_handler;
	sigemptyset(&siggy.sa_mask);
	siggy.sa_flags = SA_SIGINFO;

	/* Install the SIGQUIT handler */
	return (sigaction(SIGQUIT, &siggy, NULL));
}
