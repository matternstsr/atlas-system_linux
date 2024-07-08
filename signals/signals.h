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

/* Function to retrieve current handler of SIGINT using sigaction */
void (*current_handler_sigaction(void))(int);

/* Global variable to store the PID of the sender */
void sigquit_handler(int signum, siginfo_t *info, void *context);

/* Function to set up signal handler for SIGQUIT */
int trace_signal_sender(void);

/* Signal handler for SIGINT */
void sigint_handler(int signum);

/* Function to set up signal handler for SIGINT and handle it once */
void handle_sigint_and_exit(void);

int pid_exist(pid_t pid);

#endif /* SIGNALS_H */
