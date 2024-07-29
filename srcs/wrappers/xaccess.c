#include <unistd.h>

int	xaccess(const char *path, int mode)
{
	return (access(path, mode));
}
