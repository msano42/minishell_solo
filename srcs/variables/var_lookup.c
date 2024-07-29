#include <stddef.h>

#include "variables.h"

t_shell_var	*var_lookup(const char *name, t_var_context *context, t_globals *g)
{
	t_shell_var		*var;

	if (!name || !context || !g)
		return (NULL);
	var = NULL;
	while (context)
	{
		var = hash_lookup(name, context->table, g);
		if (var)
			break ;
		context = context->down;
	}
	return (var);
}
