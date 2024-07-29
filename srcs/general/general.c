#include <stddef.h>
#include <stdbool.h>

#include "general.h"
#include "wrappers.h" // memory

void	*expand_buffer(void *ptr, size_t size, size_t increase)
{
	if (SIZE_MAX - size < increase)
	{
		free(ptr);
		return (NULL);
	}
	return (ft_realloc(ptr, size, size + increase));
}

bool	is_backslash_newline(const char *str)
{
	if (!str)
		return (false);
	return (str[0] == '\\' && str[1] == '\n');
}
