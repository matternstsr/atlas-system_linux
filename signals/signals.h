#ifndef SIGNALS_H
#define SIGNALS_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

/* Function to set up signal handler for SIGINT */
int handle_signal(void);

void sigint_handler(int signum);

/* Function to retrieve current handler of SIGINT */
void (*current_handler_signal(void))(int);

#endif /* SIGNALS_H */
