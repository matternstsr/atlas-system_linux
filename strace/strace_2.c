#include "syscalls.h"

/**
 * main - The entry point of the program
 * @argc: The number of command-line arguments passed to the program.
 * @argv: An array of strings representing the command-line arguments.
 * @envp: An array of strings representing the environment variables.
 * 
 * This function creates a child process, attaches a tracer to it, and executes
 * the specified program within the child process. The parent process monitors
 * the child process, prints the system call names and their return values, and 
 * handles termination of the child process.
 * 
 * Return: Returns 0 on successful execution, or exits with status 1 if an error occurs.
 */
int main(int argc, const char *argv[], char *const envp[])
{
    pid_t child;
    int status, syscall_count = 0;
    struct user_regs_struct regs;
    // long syscall_num;
    // size_t num_syscalls = sizeof(syscalls_64_g) / sizeof(syscalls_64_g[0]);

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1; // Return 1 to indicate an error in usage
    }

    child = fork();
    if (child == -1)
    {
        perror("fork");
        return 1; // Return 1 to indicate an error in fork
    }

    if (child == 0)
    {
        // In the child process
        // if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
        // {
        //     perror("ptrace");
        //     return 1; // Return 1 to indicate an error in ptrace
        // }
        ptrace(PTRACE_TRACEME, child, NULL, NULL);
        execve(argv[1], (char *const *)(argv + 1), (char *const *)envp);
        perror("execve"); // Handle execve error
        return 1; // Return 1 to indicate an error in execve
    }
    else
    {
        while (2)
        {
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            wait(&status);
            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            if (WIFEXITED(status))
                break;

            if (syscall_count == 0 || syscall_count % 2 != 0)
				fprintf(stderr, "%s", NAMES);
			if (syscall_count % 2 == 0)
			{
				if (regs.orig_rax != 1)
					fprintf(stderr, " = %#lx\n", (size_t)regs.rax);
				else
					fprintf(stderr, " = %#lx\n", (size_t)regs.rax);
			}
            syscall_count++;
        }
    }

    return 0;
}
