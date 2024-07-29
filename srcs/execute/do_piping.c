#include <unistd.h>

#include "execute.h"
#include "general.h"

#define PIPE_DUP_ERR_FORMAT "minishell: cannot duplicate fd %d to fd %d\n"

static void	dup_error(int old_fd, int new_fd);

void	do_piping(t_pipes pipes)
{
	if (pipes.in != NO_PIPE)
	{
		if (dup2(pipes.in, 0) < 0)
			dup_error(pipes.in, 0);
		if (pipes.in > 0)
			close(pipes.in);
	}
	if (pipes.out != NO_PIPE)
	{
		if (dup2(pipes.out, 1) < 0)
			dup_error(pipes.out, 1);
		if (pipes.out == 0 || pipes.out > 1)
			close(pipes.out);
	}
}

static void	dup_error(int old_fd, int new_fd)
{
	ft_dprintf(STDERR_FILENO, PIPE_DUP_ERR_FORMAT, old_fd, new_fd);
}
