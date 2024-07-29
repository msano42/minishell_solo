#include <unistd.h>

char	*xttyname(int fd)
{
	return (ttyname(fd));
}
