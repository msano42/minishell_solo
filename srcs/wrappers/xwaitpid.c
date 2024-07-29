#include <sys/wait.h>

pid_t	xwaitpid(pid_t pid, int *stat_loc, int options)
{
	return (waitpid(pid, stat_loc, options));
}
