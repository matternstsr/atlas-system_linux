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
    int syscall_in_progress = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1;
    }

    // Create the child process
    if ((child = fork()) == -1)
    {
        perror("fork");
        return 1;
    }

    if (child == 0)
    {
        // In child process
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
        {
            perror("ptrace");
            return 1;
        }
        execve(argv[1], &argv[1], envp);
        perror("execve");
        return 1;
    }
    else
    {
        // In parent process
        while (1)
        {
            // Wait for child to stop
            if (waitpid(child, &status, 0) == -1)
            {
                perror("waitpid");
                return 1;
            }

            if (WIFEXITED(status))
                break;

            if (WIFSIGNALED(status))
            {
                fprintf(stderr, "Child process terminated by signal\n");
                return 1;
            }

            if (WIFSTOPPED(status))
            {
                // Get the syscall number
                struct user_regs_struct regs;
                if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
                {
                    perror("ptrace");
                    return 1;
                }

                // Print the syscall number if a syscall is in progress
                if (syscall_in_progress)
                {
                    print_syscall(regs.orig_rax);
                }

                // Toggle the syscall_in_progress flag
                // The ptrace(PTRACE_SYSCALL, child, NULL, NULL) call sets the child process to stop at both syscall entry and exit points. To handle this properly, you need a way to distinguish between these two events. The syscall_in_progress flag helps manage this by toggling its state each time you stop at a syscall.
                // Without this flag, the same syscall could be reported multiple times or out of sequence. By toggling the flag, you ensure that each syscall is reported once per entry (and optionally per exit if needed).
                // Initialization: When starting, the flag (syscall_in_progress) is initialized to 0 (false), meaning no syscall is currently being tracked.
                // Handling Syscall Events:
                // On Syscall Entry: When ptrace stops the child process at a syscall entry, the flag is toggled to 1 (true), indicating that the syscall number should be printed on subsequent stops.
                // On Syscall Exit: The flag is toggled back to 0 (false) when the process stops at the syscall exit. This means the syscall number won't be printed again during this stop.
                
                syscall_in_progress = !syscall_in_progress;

                // Continue to the next syscall
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
