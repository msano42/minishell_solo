#include <stddef.h>

#include "variables.h"
#include "wrappers.h" // memory

t_shell_var	*bind_variable_internal(const char *name, char *value, \
									t_bind_args args, t_globals *g)
{
	t_shell_var	*var;
	char		*new_value;

	if (!name || !g || !args.table)
		return (NULL);
	var = NULL;
	if (!(args.hflags & HASH_NOSRCH))
		var = hash_lookup(name, args.table, g);
	if (!var)
	{
		var = new_variable(name, args.table);
		if (!var)
			return (NULL);
	}
	new_value = make_variable_value(var, value, args.aflags);
	if (value && !new_value)
		return (NULL);
	free(var->value);
	var->value = new_value;
	free(var->exportstr);
	var->exportstr = NULL;
	var->attributes &= ~(ATT_INVISIBLE);
	if (is_exported_var(var))
		g->array_needs_making = true;
	return (var);
}

	// if (g->mark_modified_vars)
	// 	var->attributes |= ATT_EXPORTED;

t_bind_args	gen_bind_args(t_hash_table *table, uint8_t hflags, uint8_t aflags)
{
	return ((t_bind_args){.table = table, .hflags = hflags, .aflags = aflags});
}
