#include <sys/wait.h>

pid_t	xwait4(pid_t pid, int *stat_loc, int options, struct rusage *rusage)
{
	return (wait4(pid, stat_loc, options, rusage));
}
