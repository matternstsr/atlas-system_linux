#include "syscalls.h"

// Main function
int main(int argc, const char *argv[], char *const envp[])
{
    pid_t child;
    int status, syscall_count = 0;
    size_t i = 0;
    struct user_regs_struct regs;

    if (argc < 2)
    {
        fprintf(stderr, "Unsupported number of Arguments\n");
        return (-1);
    }

    child = fork();
    if (child == 0)
    {
        ptrace(PTRACE_TRACEME, child, NULL, NULL);
        execve(argv[1], (char * const *)(argv + 1), (char * const *)envp);
    }
    else
    {
        while (1)
        {
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            wait(&status);
            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            if (WIFEXITED(status))
            {
                fprintf(stderr, ") = ?\n");
                break;
            }
            if (syscall_count == 0 || syscall_count % 2 != 0)
            {
                fprintf(stderr, "%s(", NAMES);
                for (i = 0; i < PARAMETERS; i++)
                {
                    if (TYPES == VOID)
                        continue;
                    if (TYPES == VARARGS)
                        fprintf(stderr, ", ...");
                    else
                        print_param_functions[i](&regs);
                }
            }
            if (syscall_count % 2 == 0)
                fprintf(stderr, ") = %#lx\n", (size_t)regs.rax);
            syscall_count++;
        }
    }
    return (0);
}

// Functions to print specific registers
void print_rdi(struct user_regs_struct *regs)
{
    fprintf(stderr, "%#lx", (size_t)regs->rdi);
}

void print_rsi(struct user_regs_struct *regs)
{
    fprintf(stderr, ", %#lx", (size_t)regs->rsi);
}

void print_rdx(struct user_regs_struct *regs)
{
    fprintf(stderr, ", %#lx", (size_t)regs->rdx);
}

void print_r10(struct user_regs_struct *regs)
{
    fprintf(stderr, ", %#lx", (size_t)regs->r10);
}

void print_r8(struct user_regs_struct *regs)
{
    fprintf(stderr, ", %#lx", (size_t)regs->r8);
}

void print_r9(struct user_regs_struct *regs)
{
    fprintf(stderr, ", %#lx", (size_t)regs->r9);
}

static inline int get_regs(pid_t child, struct user_regs_struct *regs)
{
    return ptrace(PTRACE_GETREGS, child, NULL, regs) == -1 ? -1 : 0;
}

static inline int should_print(int check)
{
    return check == 0 || (check % 2 != 0);
}
