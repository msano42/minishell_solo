#include <signal.h>

#include "sig.h"
#include "minishell.h"
#include "wrappers.h"

t_sig_handler	*set_signal_handler(int sig, t_sig_handler *handler)
{
	struct sigaction	act;
	struct sigaction	oact;

	act.sa_handler = handler;
	act.sa_flags = 0;
	xsigemptyset(&act.sa_mask);
	xsigemptyset(&oact.sa_mask);
	if (xsigaction(sig, &act, &oact) == SUCCESS)
		return (oact.sa_handler);
	else
		return (SIG_DFL);
}
