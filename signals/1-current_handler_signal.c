#include "signals.h"

/* Function to retrieve current handler of SIGINT */
void (*current_handler_signal(void))(int)
{
	void (*current_handler)(int) = signal(SIGINT, SIG_DFL);
	signal(SIGINT, current_handler);
	return current_handler;
}
