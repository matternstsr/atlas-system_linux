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
    int status, alt = 0;
    syscall_struct regs;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1;
    }

    if ((child = fork()) == -1) {
        perror("fork");
        return 1;
    }
    if (child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        if (execve(argv[1], &argv[1], envp) == -1) {
            perror("execve");
            return 1;
        }
    } else {
        while (1) {
            wait(&status);
            if (WIFEXITED(status))
                break;
            if (get_regs(child, &regs) == 0 && should_print(alt)) {
                printf("%lu\n", (unsigned long)regs.orig_rax);
                fflush(stdout);
            }
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            alt++;
        }
    }
    return 0;
}

static inline int get_regs(pid_t pid, syscall_struct *regs)
{
    return ptrace(PTRACE_GETREGS, pid, NULL, regs);
}

static inline int should_print(int alt)
{
    return !alt || (alt & 1);
}
