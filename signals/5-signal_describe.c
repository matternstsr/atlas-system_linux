#include "signals.h"

/**
 * describe - Prints the description of a given signal number
 * @argc: Number of arguments
 * @argv: Array of arguments (including the program name and signal number)
 *
 * This program takes one argument, which is the signal number to be described.
 * It prints the description of the signal corresponding to the provided
 * signal number.
 * If the number of arguments is incorrect or the signal number
 * is out of range, it prints the correct usage and exits with EXIT_FAILURE.
 *
 * Return: EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int main(int argc, char *argv[])
{

	int signum;
	const char *description;

	if (argc != 2)
	{
		printf("Usage: %s <signum>\n", argv[0]);
		return EXIT_FAILURE;
	}

	signum = atoi(argv[1]);

	if (signum < 1 || signum >= NSIG)
	{
		printf("%d: %s\n", signum, strsignal(signum));
		return EXIT_FAILURE;
	}

	description = strsignal(signum);
	if (description == NULL)
	{
		printf("%d: %s %d\n", signum, strsignal(signum), signum);
		return EXIT_FAILURE;
	}

	printf("Signal %d: %s\n", signum, description);

	return EXIT_SUCCESS;
}
