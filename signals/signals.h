#ifndef SIGNALS_H
#define SIGNALS_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

/* Function to set up signal handler for SIGINT */
int handle_signal(void);

/* Function to retrieve current handler of SIGINT */
void (*current_handler_signal(void))(int);

/* Function to set up signal handler for SIGINT using sigaction */
int handle_sigaction(void);

/* Signal handler for SIGINT */
void sigint_handler(int signum);

#endif /* SIGNALS_H */
