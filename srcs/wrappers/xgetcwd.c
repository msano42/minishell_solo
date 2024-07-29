#include <unistd.h>

char	*xgetcwd(char *buf, size_t size)
{
	return (getcwd(buf, size));
}
