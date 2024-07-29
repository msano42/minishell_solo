#include <unistd.h>

int	xunlink(const char *path)
{
	return (unlink(path));
}
