#include <sys/wait.h>

sig_t	xsignal(int sig, sig_t func)
{
	return (signal(sig, func));
}
