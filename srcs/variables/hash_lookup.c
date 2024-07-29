#include <stddef.h>

#include "variables.h"

t_shell_var	*hash_lookup(const char *name, t_hash_table *table, t_globals *g)
{
	t_bucket	*bucket;

	if (!name || !table || !g)
		return (NULL);
	bucket = hash_search(name, table);
	if (bucket)
	{
		g->last_table_searched = table;
		return (bucket->data);
	}
	return (NULL);
}

/*
	Might not need last_table_searched. Only used by make_local_variable(),
	which is only called by the declare builtin (and other make_local family
	functions).
*/
