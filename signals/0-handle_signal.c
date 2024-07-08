#include <stdio.h>
#include <stdlib.h>
#include "signals.h"

/* Function to handle SIGINT signal */
void sigint_handler(int signum) {
    printf("Gotcha! [%d]\n", signum);
}

/* Function to set up signal handler for SIGINT */
int handle_signal(void) {
    /* Set the signal handler */
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        /* If an error occurs while setting the signal handler */
        return -1;
    }
    /* Return 0 on success */
    return 0;
}
