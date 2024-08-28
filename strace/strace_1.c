#include "syscalls.h"

int main(int argc, const char *argv[], char *const envp[]) {
    pid_t child;
    int status, syscall_count = 0;
    struct user_regs_struct regs;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return -1;
    }

    child = fork();
    // if (child == -1) {
    //     perror("fork");
    //     return -1;
    // }

    if (child == 0) {
        // In the child process
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
            perror("ptrace");
            return -1;
        }
		execve(argv[1], (char * const *)(argv + 1), (char * const *)envp);        perror("execve");
        // return -1;
    } else {
        while (2) {
            ptrace(PT_SYSCALL, child, NULL, NULL);
            wait(&status);
            // if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1) {
            //     perror("ptrace");
            //     // return -1;
            // }

            if (WIFEXITED(status)) {
                break;
            }
            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            // if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1) {
            //     perror("ptrace");
            //     // return -1;
            // }

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

    return 0;
}