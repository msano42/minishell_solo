#include <unistd.h>

int	xpipe(int fd[2])
{
	return (pipe(fd));
}
