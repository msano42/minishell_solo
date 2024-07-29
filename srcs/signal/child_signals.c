#include <signal.h>
#include <unistd.h>

#include "sig.h"

void	init_child_sighandlers(void)
{
	set_signal_handler(SIGQUIT, SIG_DFL);
	set_signal_handler(SIGINT, SIG_DFL);
}
