#include <stdbool.h>
#include <errno.h>

#include "minishell.h"
#include "wrappers.h"
#include "general.h"

/*
	Unlike Bash:
		- Doesn't take pointers to struct stat as arguments. We always call
		stat inside the function.
*/
bool	is_same_file(const char *path1, const char *path2)
{
	struct stat	st1;
	struct stat	st2;

	errno = 0;
	if (!path1 || !path2)
		return (false);
	if (xstat(path1, &st1) != SUCCESS)
		return (false);
	if (xstat(path2, &st2) != SUCCESS)
		return (false);
	return ((st1.st_dev == st2.st_dev) && (st1.st_ino == st2.st_ino));
}

/*
	Unlike Bash:
		- Doesn't handle CYGWIN.
*/
bool	is_directory(const char *path)
{
	struct stat	buf;

	errno = 0;
	if (xstat(path, &buf) != SUCCESS)
		return (false);
	return (S_ISDIR(buf.st_mode));
}

bool	is_absolute_program(const char *path)
{
	if (!path)
		return (false);
	return (ft_strchr(path, '/') != NULL);
}

bool	is_valid_fd(int fd)
{
	struct stat	buf;

	errno = 0;
	fstat(fd, &buf);
	return (errno != EBADF);
}
