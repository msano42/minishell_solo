#include <unistd.h>
#include <stddef.h>
#include <limits.h>

void	close_fd_bitmap(char *bitmap)
{
	size_t	fd;

	if (!bitmap)
		return ;
	fd = 0;
	while (fd < OPEN_MAX + 1)
	{
		if (bitmap[fd])
		{
			close(fd);
			bitmap[fd] = 0;
		}
		fd++;
	}
}
