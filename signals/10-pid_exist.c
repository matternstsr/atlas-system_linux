#include <sys/types.h>
#include <signal.h>

int pid_exist(pid_t pid)
{
	return (!kill(pid, 0));
}
