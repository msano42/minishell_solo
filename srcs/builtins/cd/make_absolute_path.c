#include <stddef.h>

#include "general.h"

/**
	@fn char	*make_absolute_path(const char *str, const char *dot_path)

	@brief Constructs the absolute path of \ar str.

	If \ar str is already an absolute path (ie. starts with '/') or 
	\ar dot_path is NULL, returns a copy of \ar str.

	Otherwise, calls make_path().

	@param str A relative or absolute path.
	@param dot_path Symbolic location of ' . '.

	@return Pointer to the allocated absolute path if successful.
	NULL if allocation fails or \ar string is NULL.

	@showrefby
*/
char	*make_absolute_path(const char *str, const char *dot_path)
{
	if (!str)
		return (NULL);
	if (!dot_path || str[0] == '/')
		return (ft_strdup(str));
	return (join_path(dot_path, str, 0));
}
