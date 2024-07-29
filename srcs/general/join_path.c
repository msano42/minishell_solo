#include <stddef.h>

#include "general.h"

char	*join_path(const char *path, const char *dir, uint8_t flags)
{
	if (!path || *path == '\0')
		path = ".";
	else if ((flags & MP_IGNDOT) && path[0] == '.' && (path[1] == '\0' \
			|| (path[1] == '/' && path[2] == '\0')))
		path = "";
	if (!dir)
		dir = "";
	if ((flags & MP_RMDOT) && dir[0] == '.' && dir[1] == '/')
		dir += 2;
	if (path[0] && path[ft_strlen(path) - 1] != '/')
		return (join_with_separator(path, dir, '/'));
	return (ft_strjoin(path, dir));
}
