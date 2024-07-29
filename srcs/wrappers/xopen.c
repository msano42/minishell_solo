#include <fcntl.h>

#include "wrappers.h"

int	xopen(const char *path, int oflag, mode_t mode)
{
	if (mode == NO_MODE)
		return (open(path, oflag));
	return (open(path, oflag, mode));
}
