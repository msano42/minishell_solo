#include <unistd.h>

#include "execute.h"

void	close_pipes(t_pipes pipes)
{
	if (pipes.in >= 0)
		close(pipes.in);
	if (pipes.out >= 0)
		close(pipes.out);
}
