#include <signal.h>

int	xkill(pid_t pid, int sig)
{
	return (kill(pid, sig));
}
