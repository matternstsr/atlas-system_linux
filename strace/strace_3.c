#include "syscalls.h"

// Function prototypes for printing registers
void print_rdi(struct user_regs_struct *regs);
void print_rsi(struct user_regs_struct *regs);
void print_rdx(struct user_regs_struct *regs);
void print_r10(struct user_regs_struct *regs);
void print_r8(struct user_regs_struct *regs);
void print_r9(struct user_regs_struct *regs);

// Array of function pointers to print specific registers
void (*print_param_functions[MAX_PARAMS])(struct user_regs_struct *) = {
    print_rdi,
    print_rsi,
    print_rdx,
    print_r10,
    print_r8,
    print_r9
};

// Main function
int main(int argc, const char *argv[], char *const envp[])
{
    pid_t child;
    int status, print_check = 0;
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
            if (print_check == 0 || print_check % 2 != 0)
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
            if (print_check % 2 == 0)
                fprintf(stderr, ") = %#lx\n", (size_t)regs.rax);
            print_check++;
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
