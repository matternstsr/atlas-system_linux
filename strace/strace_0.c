#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>

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
int main(int argc, const char *argv[], char *const envp[])
{
    pid_t child;
    int status, print_check = 0;
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
        execve(argv[1], (char *const *)(argv + 1), (char *const *)envp);
        perror("execve"); // Handle execve error
        return 1; // Return 1 to indicate an error in execve
    } else {
        // In the parent process
        while (1) {
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            wait(&status);
            if (WIFEXITED(status))
                break;

            // Retrieve and print system call numbers
            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            if (print_check == 0 || print_check % 2 != 0) {
                fprintf(stderr, "%lu\n", (unsigned long)regs.orig_rax);
            }
            print_check++;
        }
    }
    return 0;
}
