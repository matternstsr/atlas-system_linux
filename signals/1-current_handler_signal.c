#include "signals.h"

/**
 * current_handler_signal - Retrieve current handler for SIGINT
 * 
 * This function temporarily sets the SIGINT signal handler to SIG_DFL
 * (default handler), retrieves the current handler, and restores the
 * original handler. It returns a pointer to the current SIGINT signal
 * handler function.
 *
 * Return:
 *   Pointer to the current SIGINT signal handler function, or NULL on failure.
 */
void (*current_handler_signal(void))(int)
{
	void (*current_handler)(int) = signal(SIGINT, SIG_DFL);
	signal(SIGINT, current_handler);
	return current_handler;
}
