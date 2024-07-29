#include <unistd.h>

ssize_t	xwrite(int fd, const void *buf, size_t nbyte)
{
	return (write(fd, buf, nbyte));
}
