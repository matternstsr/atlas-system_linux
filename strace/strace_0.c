#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void print_syscall(long syscall_num)
{
    printf("%ld\n", syscall_num);
}

int main(int argc, char *argv[])
{
    pid_t child;
    int status;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1;
    }
    // Create process
    if ((child = fork()) == -1)
    {
        perror("fork");
        return 1;
    }
    if (child == 0)
    { // If child process
        // Make sure its searchable
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
        {
            perror("ptrace");
            return 1;
        }
        // Replace old with the new processes
        execvp(argv[1], &argv[1]);
        perror("execvp");
        return 1;
    }
    else // If its a parent process
    {
        // Stop and wait for the slow child
        while (1)
        {
            waitpid(child, &status, 0);
            if (WIFEXITED(status)) break;
            if (WIFSIGNALED(status))
            {
                fprintf(stderr, "Child process terminated by signal\n");
                return 1;
            }
            if (WIFSTOPPED(status))
            {
                // Grab the call number
                struct user_regs_struct regs;
                if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
                {
                    perror("ptrace");
                    return 1;
                }
                // Print the call number
                print_syscall(regs.orig_rax);
                // Continue child process
                if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
                {
                    perror("ptrace");
                    return 1;
                }
            }
        }
    }
    return 0;
}