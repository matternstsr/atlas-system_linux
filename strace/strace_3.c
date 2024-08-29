#include "syscalls.h"

int main(int argc, char *argv[])
{
    pid_t child;
    int status;
    struct user_regs_struct regs;

    // Argument Parsing:
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1;
    }
    // Forking the Process:
    child = fork();
    if (child == -1)
    {
        perror("fork");
        return 1;
    }
    // Child Process Setup:
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

            // Print call name and parameters
            // Syscall Information Extraction:
            if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
            {
                perror("ptrace(GETREGS)");
                return 1;
            }

            // Ensure the syscall number is within bounds
            // Syscall Name and Parameters:
            size_t syscall_num = (size_t)regs.orig_rax;
            if (syscall_num < sizeof(syscalls_64_g) / sizeof(syscalls_64_g[0])) {
                printf("%s(", syscalls_64_g[syscall_num].name);

                // Print parameters based on call number and type
                for (int i = 0; i < (int)PARAMETERS; i++)
                {
                    if (i > 0)
                        printf(", ");

                    switch (i)
                    {
                        case 0: printf("%lx", (unsigned long)regs.rdi); break;
                        case 1: printf("%lx", (unsigned long)regs.rsi); break;
                        case 2: printf("%lx", (unsigned long)regs.rdx); break;
                        case 3: printf("%lx", (unsigned long)regs.r10); break;
                        case 4: printf("%lx", (unsigned long)regs.r8); break;
                        case 5: printf("%lx", (unsigned long)regs.r9); break;
                        default: break; // You can handle more cases if needed
                    }
                }
                printf(") = %lx\n", (unsigned long)regs.rax);
            } else {
                printf("unknown_syscall(%lx) = %lx\n", (unsigned long)regs.orig_rax, (unsigned long)regs.rax);
            }
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