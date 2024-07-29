#include <stdbool.h>
#include <stddef.h>

#include "general.h"

bool	has_unprintable_chars(const char *str)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (!ft_isprint(*str))
			return (true);
		str++;
	}
	return (false);
}
