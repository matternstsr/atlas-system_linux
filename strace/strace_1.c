#include "syscalls.h"

extern const char *syscall_names[];

// int main(int argc, char *argv[])
int main(int argc, const char *argv[], char *const envp[])
{
    pid_t child;
    int status;
    struct user_regs_struct regs;

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
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execve(argv[1], (char *const *)(argv + 1), (char *const *)envp);
        perror("execve"); // Handle execve error
        return 1; // Return 1 to indicate an error in execve
    }
    else
    {
        // In parent process
        while (1)
        {
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            wait(&status);
            if (WIFEXITED(status))
                break;

            // Print syscall name
            if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
            {
                perror("ptrace(GETREGS)");
                return 1;
            }
            size_t syscall_nr = (size_t)regs.orig_rax;
            if (syscall_nr < sizeof(syscall_names) / sizeof(syscall_names[0])) {
                printf("%s\n", syscall_names[syscall_nr]);
            } else {
                printf("unknown syscall %zu\n", syscall_nr);
            }
        }
    }
    return 0;
}
