#include "general.h"
#include "execute.h"
#include "execute_internal.h"
#include "wrappers.h" // memory

char	*copy_fd_bitmap(const char *bitmap)
{
	char	*new;

	new = xmalloc(OPEN_MAX + 1);
	if (!new)
		return (NULL);
	ft_memmove(new, bitmap, OPEN_MAX + 1);
	return (new);
}

bool	is_pipe(t_command *command)
{
	if (!command)
		return (false);
	if (command->type != CM_CONNECTION || !command->u_value.connection)
		return (false);
	return (command->u_value.connection->connector == '|');
}

int	execute_pipeline(t_command *command, t_pipes pipes, char *fds_to_close, t_globals *g)
{
	int		result;
	int		fds[2];
	int		prev_pipe;
	t_pipes	pipes_local;
	char	*fd_bitmap;

	prev_pipe = pipes.in;
	while (is_pipe(command))
	{
		if (pipe(fds) < 0)
		{
			// figure this out later
			g->last_command_exit_value = EXECUTION_FAILURE;
			return (g->last_command_exit_value);
		}
		fd_bitmap = copy_fd_bitmap(fds_to_close);
		fd_bitmap[fds[0]] = 1;
		pipes_local.in = prev_pipe;
		pipes_local.out = fds[1];
		execute_command_internal(command->u_value.connection->left, pipes_local, fd_bitmap, g);
		if (prev_pipe >= 0)
			close(prev_pipe);
		prev_pipe = fds[0];
		close(fds[1]);
		free(fd_bitmap);
		command = command->u_value.connection->right;
	}
	pipes_local.in = prev_pipe;
	pipes_local.out = pipes.out;
	result = execute_command_internal(command, pipes_local, fds_to_close, g);
	if (prev_pipe >= 0)
		close(prev_pipe);
	return (result);
}
