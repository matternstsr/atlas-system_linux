// #include "syscalls.h"

// /**
//  * main - The entry point of the program
//  * @argc: The number of command-line arguments passed to the program.
//  * @argv: An array of strings representing the command-line arguments.
//  * @envp: An array of strings representing the environment variables.
//  * 
//  * This function creates a child process, attaches a tracer to it, and executes
//  * the specified program within the child process. The parent process monitors
//  * the child process, prints the system call numbers made by the child, and 
//  * handles termination of the child process.
//  * 
//  * Return: Returns 0 on successful execution, or exits with status 1 if an error occurs.
//  */
// int main(int argc, const char *argv[], char *const envp[]) {
//     pid_t child;
//     int status, syscall_count = 0;
//     struct user_regs_struct regs;

//     if (argc < 2) {
//         fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
//         return 1; // Return 1 to indicate an error in usage
//     }

//     child = fork();
//     // if (child == -1) {
//     //     perror("fork");
//     //     return -1;
//     // }

//     if (child == 0) {
//         // In the child process
//         if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
//             perror("ptrace");
//             return 1; // Return 1 to indicate an error in ptrace
//         }
//         execve(argv[1], (char *const *)(argv + 1), (char *const *)envp);
//         perror("execve"); // Handle execve error
//         return 1; // Return 1 to indicate an error in execve
//     } else {
//         while (2) {
//             ptrace(PT_SYSCALL, child, NULL, NULL);
//             wait(&status);
//             // if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1) {
//             //     perror("ptrace");
//             //     // return -1;
//             // }

//             if (WIFEXITED(status)) {
//                 break;
//             }
//             ptrace(PTRACE_GETREGS, child, NULL, &regs);
//             // if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1) {
//             //     perror("ptrace");
//             //     // return -1;
//             // }

//             if (syscall_count == 0 || syscall_count % 2 != 0) {
//                 if (regs.orig_rax != 1) {
//                     fprintf(stderr, "%s\n", SYSNAME);
//                 } else {
//                     fprintf(stderr, "%s", SYSNAME);
//                 }
//             }

//             if (syscall_count % 2 == 0 && regs.orig_rax == 1) {
//                 fprintf(stderr, "\n");
//             }

//             syscall_count++;
//         }
//     }

//     return 0;
// }
#include "syscalls.h"

int main(int argc, const char *argv[], char *const envp[]) {
    pid_t child;
    int status, syscall_count = 0;
    struct user_regs_struct regs;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1; // Return 1 to indicate an error in usage
    }

    child = fork();
    if (child == -1) {
        handle_ptrace_error("fork");
    }

    if (child == 0) {
        // In the child process
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
            handle_ptrace_error("ptrace");
        }
        execve(argv[1], (char *const *)(argv + 1), (char *const *)envp);
        handle_ptrace_error("execve");
    } else {
        while (1) {
            if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1) {
                handle_ptrace_error("ptrace");
            }
            wait(&status);

            if (WIFEXITED(status)) {
                break;
            }

            if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1) {
                handle_ptrace_error("ptrace");
            }

            print_syscall_number(&regs, syscall_count);
            syscall_count++;
        }
    }

    return 0;
}


static inline void handle_ptrace_error(const char *msg) {
    perror(msg);
    // _exit(1); // Use _exit to avoid additional cleanup by stdio
}

static inline void print_syscall_number(struct user_regs_struct *regs, int syscall_count) {
    if (syscall_count == 0 || syscall_count % 2 != 0) {
        if (regs->orig_rax < PT_SYSCALL && syscalls_64_g[regs->orig_rax].name) {
            fprintf(stderr, "%s", SYSNAME);
        }
        // else {
        //     fprintf(stderr, "Unknown syscall %lld", regs->orig_rax);
        // }
    }

    if (syscall_count % 2 == 0 && regs->orig_rax < PT_SYSCALL && syscalls_64_g[regs->orig_rax].name) {
        fprintf(stderr, "\n");
    }
}
