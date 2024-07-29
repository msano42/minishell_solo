#include <sys/wait.h>

pid_t	xwait3(int *stat_loc, int options, struct rusage *rusage)
{
	return (wait3(stat_loc, options, rusage));
}
