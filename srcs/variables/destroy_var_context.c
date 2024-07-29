#include "hashlib.h"
#include "variables.h"
#include "wrappers.h" // memory

void	destroy_var_context(t_var_context *context)
{
	if (!context)
		return ;
	free(context->name);
	if (context->table)
	{
		hash_flush(context->table, free_variable_hash_data);
		hash_destroy(context->table);
	}
	free(context);
}
