#include <sys/stat.h>

int	xfstat(int fd, struct stat *buf)
{
	return (fstat(fd, buf));
}
