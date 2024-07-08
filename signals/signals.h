#ifndef SIGNALS_H
#define SIGNALS_H

/* Function to set up signal handler for SIGINT */
int handle_signal(void);

/* Function to retrieve current handler of SIGINT */
void (*current_handler_signal(void))(int);

#endif /* SIGNALS_H */
