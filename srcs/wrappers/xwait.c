#include <sys/wait.h>

pid_t	xwait(int *stat_loc)
{
	return (wait(stat_loc));
}
