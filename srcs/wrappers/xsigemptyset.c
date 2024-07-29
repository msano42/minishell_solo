#include <signal.h>

int	xsigemptyset(sigset_t *set)
{
	return (sigemptyset(set));
}
