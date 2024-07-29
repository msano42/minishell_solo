#include <stdbool.h>
#include <errno.h>
#include <stdint.h>

#include "general.h"

bool	is_legal_variable_starter(char c)
{
	return (ft_isalpha(c) || c == '_');
}

bool	is_legal_variable_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

bool	is_legal_identifier(const char *name)
{
	if (!name || !*name)
		return (false);
	if (!is_legal_variable_starter(*name))
		return (false);
	name++;
	while (*name && is_legal_variable_char(*name))
		name++;
	if (*name)
		return (false);
	return (true);
}

bool	is_legal_number(const char *string, intmax_t *result)
{
	intmax_t	value;
	char		*endptr;

	if (result)
		*result = 0;
	if (!string)
		return (false);
	errno = 0;
	value = ft_strtoimax(string, &endptr, 10);
	if (errno || endptr == string)
		return (false);
	while (is_blank_char(*endptr))
		endptr++;
	if (*string && *endptr == '\0')
	{
		if (result)
			*result = value;
		return (true);
	}
	return (false);
}
