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