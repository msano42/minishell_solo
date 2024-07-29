#include <stddef.h>

#include "general.h"
#include "variables.h"
#include "wrappers.h" // memory

static enum e_status	set_if_exists_in_tempenv(const char *name, \
												char *value, t_globals *g);

t_shell_var	*bind_variable(const char *name, char *value, uint8_t flags, \
							t_globals *g)
{
	t_shell_var			*var;
	t_bind_args			bargs;
	t_var_context		*context;

	if (!name || !g || !g->shell_variables)
		return (NULL);
	if (g->temporary_env)
	{
		if (set_if_exists_in_tempenv(name, value, g) == E_MALLOC)
			return (NULL);
	}
	context = g->shell_variables;
	while (context)
	{
		if (context->flags & VC_BLTNENV)
		{
			var = hash_lookup(name, context->table, g);
			bargs = gen_bind_args(context->table, 0, flags);
			if (var)
				return (bind_variable_internal(var->name, value, bargs, g));
		}
		context = context->down;
	}
	bargs = gen_bind_args(g->global_variables->table, 0, flags);
	return (bind_variable_internal(name, value, bargs, g));
}

static enum e_status	set_if_exists_in_tempenv(const char *name, char *value, \
											t_globals *g)
{
	t_shell_var	*var;

	if (!name || !value || !g)
		return (E_VALUE);
	var = NULL;
	if (g->temporary_env)
		var = hash_lookup(name, g->temporary_env, g);
	if (var)
	{
		free(var->exportstr);
		var->exportstr = NULL;
		free(var->value);
		var->value = NULL;
		if (value)
		{
			var->value = ft_strdup(value);
			if (!var->value)
				return (E_MALLOC);
		}
	}
	return (SUCCESS);
}
/* See Bash bind_tempenv_variable().

	Unlike Bash, does not return a pointer to the variable because it isn't
	used. Instead returns e_status for error handling malloc().
	Call hash_lookup() if you want the pointer that badly.
*/