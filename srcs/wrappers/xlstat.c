#include <sys/stat.h>

int	xlstat(const char *path, struct stat *buf)
{
	return (lstat(path, buf));
}
