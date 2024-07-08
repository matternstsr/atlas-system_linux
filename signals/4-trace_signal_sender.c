
#include <stdio.h>
#include <stdlib.h>
#include <signals.h>
#include <unistd.h>

/* Global variable to store the PID of the sender */
static volatile pid_t sender_pid;

/* Signal handler for SIGQUIT */
void sigquit_handler(int signum, siginfo_t *info, void *context) {
    if (signum == SIGQUIT) {
        // Store the PID of the sender
        sender_pid = info->si_pid;
        
        // Print the message
        printf("SIGQUIT sent by %d\n", sender_pid);
    }
}

/* Function to set up signal handler for SIGQUIT */
int trace_signal_sender(void) {
    struct sigaction sa;

    // Set up the sigaction struct
    sa.sa_sigaction = sigquit_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    // Install the SIGQUIT handler
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        perror("sigaction");
        return -1;
    }

    return 0;
}
