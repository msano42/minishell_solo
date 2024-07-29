#include <stddef.h>

#include "variables.h"

t_shell_var	*set_if_not(char *name, char *value, t_globals *g)
{
	t_shell_var	*var;
	t_bind_args	bind_args;

	if (!name || !g)
		return (NULL);
	if (!g->shell_variables)
	{
		if (create_variable_table(g) == E_MALLOC)
			return (NULL);
	}
	var = find_variable(name, g);
	if (!var)
	{
		bind_args = gen_bind_args(g->global_variables->table, HASH_NOSRCH, 0);
		var = bind_variable_internal(name, value, bind_args, g);
	}
	return (var);
}
