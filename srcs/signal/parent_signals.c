#include <signal.h>
#include <unistd.h>

#include "sig.h"

void	init_parent_sighandlers(void)
{
	set_signal_handler(SIGQUIT, SIG_IGN);
	set_signal_handler(SIGINT, SIG_IGN);
}