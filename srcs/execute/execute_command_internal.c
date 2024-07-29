#include "execute.h"
#include "execute_internal.h"
#include "wrappers.h" // memory

int	execute_command_internal(t_command *command, t_pipes pipes, \
	char *fds_to_close, t_globals *g)
{
	int	result;

	result = EXECUTION_SUCCESS;
	if (!command)
		return (EXECUTION_SUCCESS);
	if (command->type == CM_SUBSHELL)
		return (execute_subshell_command(command, pipes, fds_to_close, g));
	if (command->type == CM_SIMPLE)
	{
		g->last_made_pid = NO_PID;
		result = execute_simple_command(command->u_value.simple, pipes, \
				fds_to_close, g);
		if (g->already_making_children && pipes.out == NO_PIPE)
		{
			g->already_making_children = false;
			if (g->last_made_pid != NO_PID)
				result = wait_for(g->last_made_pid, g);
		}
	}
	if (command->type == CM_CONNECTION)
		result = execute_connection(command, pipes, fds_to_close, g);
	g->last_command_exit_value = result;
	return (result);
}
