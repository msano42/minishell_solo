#include <stddef.h>

#include "general.h"

char	*extract_colon_unit(const char *str, size_t *index_ptr)
{
	size_t	start;
	size_t	end;

	if (!str || !index_ptr)
		return (NULL);
	if (*index_ptr >= ft_strlen(str))
		return (NULL);
	start = *index_ptr;
	if (start && str[start] == ':')
		start++;
	end = start;
	while (str[end] && str[end] != ':')
		end++;
	*index_ptr = end;
	if (end == start)
	{
		if (str[end])
			(*index_ptr)++;
		return (ft_strdup(""));
	}
	return (substring(str, start, end));
}
