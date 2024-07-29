#include <sys/stat.h>

int	xstat(const char *path, struct stat *buf)
{
	return (stat(path, buf));
}
