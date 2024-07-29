#include <unistd.h>

ssize_t	xread(int fd, void *buf, size_t nbyte)
{
	return (read(fd, buf, nbyte));
}
