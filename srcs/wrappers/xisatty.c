#include <unistd.h>

int	xisatty(int fd)
{
	return (isatty(fd));
}
