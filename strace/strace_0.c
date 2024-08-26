#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/**
 * print_syscall - Prints the syscall number
 * @syscall_num: The number of the syscall
 */
void print_syscall(long syscall_num)
{
    printf("%ld\n", syscall_num);
}

int main(int argc, char *argv[], char *const envp[])
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
    {
        // If child process
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
        {
            perror("ptrace");
            return 1;
        }
        // Replace old process with new
        execve(argv[1], &argv[1], envp);
        perror("execve");
        return 1;
    }
    else // If parent process
    {
        while (1)
        {
            // Wait for the child to stop
            waitpid(child, &status, 0);

            if (WIFEXITED(status))
                break;

            if (WIFSIGNALED(status))
            {
                fprintf(stderr, "Child process terminated by signal\n");
                return 1;
            }

            if (WIFSTOPPED(status))
            {
                struct user_regs_struct regs;
                if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
                {
                    perror("ptrace");
                    return 1;
                }

                // Print the syscall number
                print_syscall(regs.orig_rax);

                // Continue the child process to the next syscall
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
