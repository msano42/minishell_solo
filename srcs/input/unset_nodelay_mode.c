#include "wrappers.h"
#include <sys/ioctl.h>

int	unset_nodelay_mode(int fd)
{
	int	opt;

	opt = 0;
	return (xioctl(fd, FIONBIO, &opt));
}
