#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>

/**
 * main - Entry point of the program
 * @argc: Number of command-line arguments
 * @argv: Array of command-line arguments
 * @envp: Array of environment variables
 * 
 * Return: Returns -1 on error, otherwise 0
 */
int main(int argc, char *argv[], char *envp[])
{
    pid_t pid;
    int status, counter = 0;
    struct user_regs_struct regs;

    if (argc < 2)
    {
        fprintf(stderr, "Error: Insufficient arguments\n");
        return -1;
    }

    pid = fork();
    if (pid == 0)
    {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execve(argv[1], argv + 1, envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else
    {
        while (1)
        {
            ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
            wait(&status);
            if (WIFEXITED(status))
                break;
            ptrace(PTRACE_GETREGS, pid, NULL, &regs);
            if (counter % 2 == 0)
                fprintf(stderr, "%lu\n", (unsigned long)regs.orig_rax);
            counter++;
        }
    }
    return 0;
}
