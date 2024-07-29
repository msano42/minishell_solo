#include <stddef.h>

#include "variables.h"
#include "general.h"

/*
	Never pass a NULL VALUE, because it can't be distinguished
	from allocation error.
*/
char	*make_variable_value(t_shell_var *var, char *value, uint8_t flags)
{
	if (!value)
		return (NULL);
	if (flags & ASS_APPEND)
	{
		if (var && var->value)
			return (ft_strjoin(var->value, value));
	}
	return (ft_strdup(value));
}
