#include <unistd.h>

int	xchdir(const char *path)
{
	return (chdir(path));
}
