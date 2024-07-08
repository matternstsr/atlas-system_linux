#include <sys/types.h>
#include <signal.h>

/**
 * pid_exist - Check
 * 
 * @pid: The pid
 * Return: 0 or 1
 */
int pid_exist(pid_t pid)
{
    return (kill(pid, 0) != 0);
}
