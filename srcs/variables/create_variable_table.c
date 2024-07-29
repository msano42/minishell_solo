#include <stddef.h>

#include "variables.h"

#define VARIABLES_HASH_BUCKETS 1024

enum e_status	create_variable_table(t_globals *g)
{
	if (!g)
		return (E_VALUE);
	if (!g->shell_variables)
	{
		g->shell_variables = new_var_context(NULL, 0, g->variable_context);
		if (!g->shell_variables)
			return (E_MALLOC);
		g->global_variables = g->shell_variables;
		g->shell_variables->scope = 0;
		g->shell_variables->table = hash_create(VARIABLES_HASH_BUCKETS);
		if (!g->shell_variables->table)
		{
			destroy_var_context(g->shell_variables);
			g->shell_variables = NULL;
			g->global_variables = NULL;
			return (E_MALLOC);
		}
	}
	return (SUCCESS);
}
