#include <stdbool.h>

#include "general.h"

/**
	@fn bool	is_absolute_path_for_cd(const char *path)

	@brief Checks if \ar path should be treated as an absolute path by
	the cd builtin.

	@param path The path to be checked.

	@return \ar true if the path is an absolute path, \ar false otherwise.

	@seebash @verbatim absolute_pathname() @endverbatim

	@showrefby
*/
bool	is_absolute_path_for_cd(const char *path)
{
	if (!path || !*path)
		return (false);
	if (path[0] == '/')
		return (true);
	if (path[0] == '.' && is_path_sep_elem(path[1]))
		return (true);
	if (is_dotdot_elem(path))
		return (true);
	return (false);
}
