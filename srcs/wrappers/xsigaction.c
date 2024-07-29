#include <signal.h>

int	xsigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	return (sigaction(sig, act, oact));
}
