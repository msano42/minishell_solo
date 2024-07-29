#include <stddef.h>

#include "variables.h"
#include "general.h"
#include "wrappers.h" // memory

t_var_context	*new_var_context(const char *name, uint8_t flags, int scope)
{
	t_var_context	*context;

	context = xmalloc(sizeof(t_var_context));
	if (!context)
		return (NULL);
	context->name = NULL;
	if (name)
	{
		context->name = ft_strdup(name);
		if (!context->name)
		{
			free(context);
			return (NULL);
		}
	}
	context->scope = scope;
	context->flags = flags;
	context->up = NULL;
	context->down = NULL;
	context->table = NULL;
	return (context);
}
