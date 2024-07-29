#include <stdbool.h>

#include "minishell.h"

bool	is_path_sep_elem(char c)
{
	return (c == '/' || c == '\0');
}

bool	is_dotdot_elem(const char *path)
{
	if (!path)
		return (false);
	return (path[0] == '.' && path[1] == '.' && is_path_sep_elem(path[2]));
}

bool	is_slash_or_dotsep_elem(const char *path)
{
	if (!path)
		return (false);
	return (*path == '/' || (*path == '.' && is_path_sep_elem(path[1])));
}

bool	is_double_slash_elem(const char *path)
{
	if (!path)
		return (false);
	return (path[0] == '/' && path[1] == '/' && path[2] != '/');
}
