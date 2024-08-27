#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

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
        return 1; // Return 1 to indicate an error in usage
    }

    child = fork();
    if (child == -1) {
        perror("fork");
        return 1; // Return 1 to indicate an error in fork
    }

    if (child == 0) {
        // In the child process
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execve(argv[1], (char * const *)(argv + 1), (char * const *)envp);
        perror("execve"); // Handle execve error
        return 1; // Return 1 to indicate an error in execve
    } else {
        // In the parent process
        while (1) {
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            wait(&status);
            if (WIFEXITED(status))
                break;
            ptrace(PTRACE_GETREGS, child, NULL, &regs);

            if (check == 0 || check % 2 != 0) {
                fprintf(stderr, "%lu\n", (unsigned long)regs.orig_rax);
                fflush(stderr); // Flush stderr to ensure output is written
            }
            check++;
        }
    }
    return 0;
}

/**
 * get_regs - Retrieve the registers of the child process using ptrace
 * @child: The PID of the child process to retrieve registers from.
 * @regs: A pointer to a user_regs_struct to store the register values.
 * 
 * Return: 0 on success, or -1 on failure.
 */
static inline int get_regs(pid_t child, user_regs *regs)
{
    return ptrace(PTRACE_GETREGS, child, NULL, regs);
}

/**
 * should_print - Determine if the system call number should be printed
 * @check: The current check value to decide printing logic.
 * 
 * Return: 1 if the system call number should be printed, 0 otherwise.
 */
static inline int should_print(int check)
{
    return !check || (check & 1);
}
