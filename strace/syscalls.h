#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>

#define SYSNAME syscalls_64_g[regs.orig_rax].name
#define SYSPARAM syscalls_64_g[regs.orig_rax].nb_params
#define SYSTYPE syscalls_64_g[regs.orig_rax].params[i]

#define MAX_PARAMS 6

/**
 * enum type_e - Enumerates the different types present in the different
 * syscall parameters and return types
 */
typedef enum type_e
{
	AIO_CONTEXT_T,
	AIO_CONTEXT_T_P,
	CADDR_T,
	CAP_USER_DATA_T,
	CAP_USER_HEADER_T,
	CHAR_P,
	CHAR_PP,
	CLOCKID_T,
	CLOCK_T,
	CPU_SET_T_P,
	DEV_T,
	ENUM___PTRACE_REQUEST,
	FD_SET_P,
	GID_T,
	GID_T_P,
	IDTYPE_T,
	ID_T,
	INT,
	INT_P,
	KEY_SERIAL_T,
	KEY_T,
	LOFF_T_P,
	LONG,
	LONG_P,
	MODE_T,
	MQD_T,
	NFDS_T,
	OFF64_T,
	OFF_T,
	OFF_T_P,
	PID_T,
	SIGHANDLER_T,
	SIGINFO_T_P,
	SIGSET_T_P,
	SIZE_T,
	SIZE_T_P,
	SOCKLEN_T,
	SOCKLEN_T_P,
	SSIZE_T,
	STACK_T_P,
	STRUCT_EPOLL_EVENT_P,
	STRUCT_GETCPU_CACHE_P,
	STRUCT_IOCB_P,
	STRUCT_IOCB_PP,
	STRUCT_IOVEC_P,
	STRUCT_IO_EVENT_P,
	STRUCT_ITIMERSPEC_P,
	STRUCT_ITIMERVAL_P,
	STRUCT_KERNEL_SYM_P,
	STRUCT_KEXEC_SEGMENT_P,
	STRUCT_LINUX_DIRENT_P,
	STRUCT_MMSGHDR_P,
	STRUCT_MQ_ATTR_P,
	STRUCT_MSGHDR_P,
	STRUCT_MSQID_DS_P,
	STRUCT_NFSCTL_ARG_P,
	STRUCT_OLD_LINUX_DIRENT_P,
	STRUCT_PERF_EVENT_ATTR_P,
	STRUCT_POLLFD_P,
	STRUCT_RLIMIT_P,
	STRUCT_ROBUST_LIST_HEAD_P,
	STRUCT_ROBUST_LIST_HEAD_PP,
	STRUCT_RUSAGE_P,
	STRUCT_SCHED_PARAM_P,
	STRUCT_SEMBUF_P,
	STRUCT_SHMID_DS_P,
	STRUCT_SIGACTION_P,
	STRUCT_SIGEVENT_P,
	STRUCT_SOCKADDR_P,
	STRUCT_STATFS_P,
	STRUCT_STAT_P,
	STRUCT_SYSINFO_P,
	STRUCT_TIMESPEC_P,
	STRUCT_TIMEVAL_P,
	STRUCT_TIMEX_P,
	STRUCT_TIMEZONE_P,
	STRUCT_TMS_P,
	STRUCT_USER_DESC_P,
	STRUCT_USTAT_P,
	STRUCT_UTIMBUF_P,
	STRUCT_UTSNAME_P,
	STRUCT_VM86_STRUCT_P,
	STRUCT___SYSCTL_ARGS_P,
	TIMER_T,
	TIMER_T_P,
	TIME_T,
	TIME_T_P,
	U64,
	UID_T,
	UID_T_P,
	UINT32_T,
	UNION_NFSCTL_RES_P,
	UNSIGNED_CHAR_P,
	UNSIGNED_INT,
	UNSIGNED_INT_P,
	UNSIGNED_LONG,
	UNSIGNED_LONG_P,
	VARARGS,
	VOID,
	VOID_P,
	VOID_PP
} type_t;

/**
 * struct syscall_s - Stores information about a syscall
 *
 * @name: Name of the syscall
 * @nr: Syscall number in the syscalls table
 * @ret: Return type
 * @nb_params: Number of parameters
 * @params: Array of parameters types
 */
typedef struct syscall_s
{
	char const * const name;
	size_t const nr;
	type_t const ret;
	size_t const nb_params;
	type_t const params[MAX_PARAMS];
} syscall_t;

// syscall_names.c
const char *syscall_names[] = {
    "read",
    "write",
    "open",
    "close",
    // Add more syscall names as needed
};

static syscall_t const syscalls_64_g[] = {
	{"read", 0, SSIZE_T, 3, {INT, VOID_P, SIZE_T, -1, -1, -1}},
	{"write", 1, SSIZE_T, 3, {INT, VOID_P, SIZE_T, -1, -1, -1}},
	{"open", 2, INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{"close", 3, INT, 1, {INT, -1, -1, -1, -1, -1}},
	{"seccomp", 317, -1, 0, {-1, -1, -1, -1, -1, -1}}
};

static syscall_t const syscalls_32_g[] = {
	{"restart_syscall", 0, INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{"exit", 1, VOID, 1, {INT, -1, -1, -1, -1, -1}},
	{"fork", 2, PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{"read", 3, SSIZE_T, 3, {INT, VOID_P, SIZE_T, -1, -1, -1}},
	{"write", 4, SSIZE_T, 3, {INT, VOID_P, SIZE_T, -1, -1, -1}},
	{"open", 5, INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{"close", 6, INT, 1, {INT, -1, -1, -1, -1, -1}},
	{"seccomp", 354, -1, 0, {-1, -1, -1, -1, -1, -1}}
};

#endif /* _SYSCALLS_H_ */