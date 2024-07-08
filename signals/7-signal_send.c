#include "signals.h"

/**
 * main - Sends SIGINT signal to a process given its PID
 * @argc: Number of arguments
 * @argv: Array of arguments (including program name and PID)
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int main(int argc, char *argv[]) {
	pid_t pid;

	if (argc != 2) {
		printf("Usage: %s <pid>\n", argv[0]);
		return EXIT_FAILURE;
	}

	pid = atoi(argv[1]);

	if (pid <= 0) {
		return EXIT_FAILURE;
	}
	if (kill(pid, SIGINT) == -1) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
