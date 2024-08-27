#include "syscalls.h"

/**
 * main - The entry point of the program
 * @argc: The number of command-line arguments passed to the program.
 * @argv: An array of strings representing the command-line arguments.
 * @envp: An array of strings representing the environment variables.
 * 
 * This function creates a child process, attaches a tracer to it, and executes
 * the specified program within the child process. The parent process monitors
 * the child process, prints the system call numbers made by the child, and 
 * handles termination of the child process.
 * 
 * Return: Returns 0 on successful execution, or exits with status 1 if an error occurs.
 */
int main(int argc, char **argv, char **envp)
{
    pid_t child;
	int status, check = 0;
	user_regs regs;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
		return (-1);
	}

	child = fork();
	if (child == -1)
		perror(argv[1]), exit(1);
	else if (child == 0)
	{
		ptrace(PTRACE_TRACEME, child, NULL, NULL);
		execve(argv[1], (char * const *)(argv + 1), (char * const *)envp);
	}
	else
	{
		while (1)
        {
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            wait(&status);
            if (WIFEXITED(status))
                break;
            ptrace(PTRACE_GETREGS, child, NULL, &regs);
			if (check == 0 || check % 2 != 0)
				fprintf(stderr, "%lu\n", (size_t)regs.orig_rax);
                fflush(stdout);
            check++;
        }
        
    }
    return 0;
}


static inline int get_regs(pid_t child, user_regs *regs)
{
    return ptrace(PTRACE_GETREGS, child, NULL, regs);
}

static inline int should_print(int check)
{
    return !check || (check & 1);
}
