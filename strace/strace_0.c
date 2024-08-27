#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define SYSCALL_EXECVE 59
#define SYSCALL_EXIT 60
#define SYSCALL_EXIT_GROUP 231

/**
 * main - The entry point of the program.
 * @argc: The number of command-line arguments passed to the program.
 * @argv: An array of strings representing the command-line arguments.
 * @envp: An array of strings representing the environment variables.
 * 
 * Creates a child process, attaches a tracer, executes the specified command,
 * and prints the system call numbers made by the child process.
 * 
 * Return: Returns 0 on success, or 1 if an error occurs.
 */
int main(int argc, char **argv, char **envp)
{
    pid_t child;
    int status;
    struct user_regs_struct regs;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1;
    }

    child = fork();
    if (child == -1) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        // In the child process
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execve(argv[1], argv + 1, envp);
        perror("execve");
        return 1;
    } else {
        // In the parent process
        while (1) {
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            wait(&status);
            if (WIFEXITED(status)) {
                break;
            }

            if (get_regs(child, &regs) == 0) {
                unsigned long syscall_num = (unsigned long)regs.orig_rax;
                if (syscall_num == SYSCALL_EXECVE || syscall_num == SYSCALL_EXIT || syscall_num == SYSCALL_EXIT_GROUP) {
                    fprintf(stderr, "%lu\n", syscall_num);
                    fflush(stderr);
                }
            }
        }
    }
    return 0;
}

/**
 * get_regs - Retrieve the registers of the child process using ptrace.
 * @child: The PID of the child process to retrieve registers from.
 * @regs: A pointer to a user_regs_struct to store the register values.
 * 
 * Return: 0 on success, or -1 on failure.
 */
static inline int get_regs(pid_t child, struct user_regs_struct *regs)
{
    return ptrace(PTRACE_GETREGS, child, NULL, regs);
}

/**
 * should_print - Determine if the system call number should be printed.
 * @check: The current check value to decide printing logic.
 * 
 * Return: 1 if the system call number should be printed, 0 otherwise.
 */
static inline int should_print(int check)
{
    return !check || (check & 1);
}
