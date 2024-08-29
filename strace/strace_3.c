#include "syscalls.h"

extern const char *syscall_names[];  // Array of calls names indexed by syscall number

int main(int argc, char *argv[])
{
    pid_t child;
    int status;
    struct user_regs_struct regs;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1;
    }

    child = fork();
    if (child == -1)
    {
        perror("fork");
        return 1;
    }

    if (child == 0)
    {
        // In child process
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(argv[1], argv + 1);
        perror("execvp");
        return 1;
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

            // Print calls name and paras
            if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
            {
                perror("ptrace(GETREGS)");
                return 1;
            }
            
            printf("%s(", syscall_names[(size_t)regs.orig_rax]);
            
            for (int i = 0; i < PARAMETERS; i++)
            {
                if (i > 0)
                    printf(", ");
                // Print params based on call num and type
                printf("%lx", (unsigned long)((void*)&regs.rdi)[i]);
            }
            printf(") = %lx\n", (unsigned long)regs.rax);
        }
    }
    return 0;
}

static inline int get_regs(pid_t child, struct user_regs_struct *regs)
{
    return ptrace(PTRACE_GETREGS, child, NULL, regs) == -1 ? -1 : 0;
}

static inline int should_print(int check)
{
    return check == 0 || (check % 2 != 0);
}