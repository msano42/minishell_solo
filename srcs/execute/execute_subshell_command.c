#include <stdlib.h>

#include "sig.h"
#include "execute.h"
#include "redirection.h"
#include "execute_internal.h"

int	execute_in_subshell(t_command *command, t_pipes pipes, char *fds_to_close, t_globals *g)
{
	g->interactive = false;
	init_parent_sighandlers();
	// reset_terminating_signals ();
	// reset_signal_handlers ();
	// set_sigint_handler ();
	// without_job_control ();
	g->last_made_pid = NO_PID;
	g->already_making_children = false;
	close_fd_bitmap(fds_to_close);
	do_piping(pipes);
	pipes.in = NO_PIPE;
	pipes.out = NO_PIPE;
	if (command->redirects)
	{
		if (do_redirections(command->redirects, 0, g) != SUCCESS)
			exit(EXECUTION_FAILURE);
		destroy_redirects(command->redirects);
		command->redirects = NULL;
	}
	if (command->type == CM_SUBSHELL)
		command = command->u_value.subshell->command;
	return (execute_command_internal(command, pipes, fds_to_close, g));
}

int	execute_subshell_command(t_command *command, t_pipes pipes, char *fds_to_close, t_globals *g)
{
	pid_t	pid;

	pid = make_child(g);
	if (pid == 0)
	{
		g->last_command_exit_value = execute_in_subshell(command, pipes, fds_to_close, g);
		exit(g->last_command_exit_value);
	}
	else
	{
		close_pipes(pipes);
		if (pipes.out != NO_PIPE)
			return (EXECUTION_SUCCESS);
		g->already_making_children = false;
		g->last_command_exit_value = wait_for(pid, 0);
		return (g->last_command_exit_value);
	}
}
