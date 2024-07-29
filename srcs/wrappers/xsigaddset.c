#include <signal.h>

int	xsigaddset(sigset_t *set, int signo)
{
	return (sigaddset(set, signo));
}
