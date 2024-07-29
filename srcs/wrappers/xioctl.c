#include <sys/ioctl.h>

int	xioctl(int fd, unsigned long request, void *ptr)
{
	return (ioctl(fd, request, ptr));
}
