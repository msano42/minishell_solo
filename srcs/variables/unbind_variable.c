#include "variables.h"
#include "wrappers.h" // memory

void	unbind_variable(const char *name, t_globals *g)
{
	t_shell_var		*var;
	t_bucket		*bucket;
	t_var_context	*context;

	bucket = NULL;
	context = g->shell_variables;
	while (context)
	{
		bucket = hash_remove(name, context->table);
		if (bucket)
			break ;
		context = context->down;
	}
	if (!bucket)
		return ;
	var = bucket->data;
	if (var)
	{
		if (is_exported_var(var))
			g->array_needs_making = true;
		handle_if_special_variable_modified(var->name, g);
	}
	free(bucket->key);
	free(bucket);
	destroy_variable(var);
}
