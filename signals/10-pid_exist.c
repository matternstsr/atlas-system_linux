#include <sys/types.h>
#include <signal.h>

/**
 * pid_exist - Check if a process exists given its PID.
 * 
 * @param pid The PID of the process to check.
 * return: 1 if the process with the PID exists, 0 otherwise.
 */
int pid_exist(pid_t pid)
{
	return (!kill(pid, 0));
}
