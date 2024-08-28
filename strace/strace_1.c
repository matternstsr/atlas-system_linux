#include "syscalls.h"

int main(int argc, const char *argv[], char *const envp[]) {
    pid_t child;
    int status, syscall_count = 0;
    struct user_regs_struct regs;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    child = fork();
    if (child == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child == 0) {
        // In the child process
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
            perror("ptrace");
            return EXIT_FAILURE;
        }
        execve(argv[1], (char *const *)(argv + 1), envp);
        perror("execve");
        return EXIT_FAILURE;
    } else {
        while (1) {
            if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1) {
                perror("ptrace");
                // return EXIT_FAILURE;
            }

            if (waitpid(child, &status, 0) == -1) {
                perror("waitpid");
                return EXIT_FAILURE;
            }

            if (WIFEXITED(status)) {
                break;
            }

            if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1) {
                perror("ptrace");
                return EXIT_FAILURE;
            }

            if (syscall_count == 0 || syscall_count % 2 != 0) {
                if (regs.orig_rax != 1) {
                    fprintf(stderr, "%s\n", SYSNAME);
                } else {
                    fprintf(stderr, "%s", SYSNAME);
                }
            }

            if (syscall_count % 2 == 0 && regs.orig_rax == 1) {
                fprintf(stderr, "\n");
            }

            syscall_count++;
        }
    }

    return EXIT_SUCCESS;
}