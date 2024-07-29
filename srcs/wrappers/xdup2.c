#include <unistd.h>

int	xdup2(int fd, int fd2)
{
	return (dup2(fd, fd2));
}
