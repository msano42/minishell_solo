#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

#include "sig.h"
#include "general.h"

static void	prompt_sighandler(int sig);

void	init_prompt_sighandlers(bool interactive)
{
	set_signal_handler(SIGQUIT, SIG_IGN);
	if (interactive)
		set_signal_handler(SIGINT, prompt_sighandler);
}

/*
	Called after prompt_sighandler() has finished executing.
	This function is executed outside of signal handler context.
*/
int	check_prompt_signals(t_globals *g)
{
	const int	sig = g_signal;

	if (g_signal == SIGINT)
	{
		if (g->last_command_exit_value < 128)
			g->last_command_exit_value = 128 + SIGINT;
		g->last_command_exit_value |= 128;
		if (g->interactive)
			ft_putstr_fd("\n", STDERR_FILENO);
	}
	g_signal = NO_SIGNAL;
	return (sig);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static void	prompt_sighandler(int sig)
{
	g_signal = sig;
	if (sig == SIGINT)
	{
		if (RL_ISSTATE(RL_STATE_SIGHANDLER))
		{
			rl_done = 1;
			rl_on_new_line();
		}
	}
}

/* **************************************************************** */
/*	                 Static Functions Documentation	            	*/
/* **************************************************************** */

/**
	@fn static void	prompt_sighandler(int sig)
	@brief Signal handler for prompting.

	\ar RL_ISSTATE(RL_STATE_SIGHANDLER) checks that we were interrupted
	during a readline() call (interactive), and not during a read()
	call (noninteractive).

	Setting \ar rl_done = 1 causes readline to return immediately.
	
	Calling rl_on_new_line() prevents cursor from writing over existing
	lines if SIGINT is received in the middle of very long (multiline) line.

	@param sig Signal number of the signal received.

	@note This function is executed in a signal handler context.

	@warning rl_done requires rl_event_hook to be non-NULL
	(see dummy_event_hook()).
	@warning rl_done must be set here to be reflected immediately, not in
	signal_event_hook or rl_event_hook for example.
*/
