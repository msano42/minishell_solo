#include "general.h"
#include "execute.h"
#include "execute_internal.h"
#include "wrappers.h" // memory

#include <stdio.h>
int	execute_command(t_command *command, t_globals *g)
{
	int			result;
	t_pipes		pipes;
	char		*fds_to_close;

	pipes.in = NO_PIPE;
	pipes.out = NO_PIPE;
	fds_to_close = xmalloc(OPEN_MAX + 1);
	if (!fds_to_close)
	{
		g->really_exit = true;
		g->last_command_exit_value = 2;
		return (2);
	}
	ft_memset(fds_to_close, 0, OPEN_MAX + 1);
	result = execute_command_internal(command, pipes, fds_to_close, g);
	free(fds_to_close);
	return (result);
}
