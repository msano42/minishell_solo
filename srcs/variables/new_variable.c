#include "general.h"
#include "hashlib.h"
#include "variables.h"
#include "wrappers.h"

static t_shell_var	*new_shell_variable(const char *name);

t_shell_var	*new_variable(const char *name, t_hash_table *table)
{
	t_shell_var	*var;
	t_bucket	*bucket;
	char		*name_cpy;

	if (!name || !table)
		return (NULL);
	var = new_shell_variable(name);
	if (!var)
		return (NULL);
	name_cpy = ft_strdup(name);
	if (!name_cpy)
	{
		destroy_variable(var);
		return (NULL);
	}
	bucket = hash_insert(name_cpy, table, HASH_NOSRCH);
	if (!bucket)
	{
		free(name_cpy);
		destroy_variable(var);
		return (NULL);
	}
	bucket->data = var;
	return (var);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static t_shell_var	*new_shell_variable(const char *name)
{
	t_shell_var	*var;

	if (!name)
		return (NULL);
	var = xmalloc(sizeof(t_shell_var));
	if (!var)
		return (NULL);
	var->name = ft_strdup(name);
	if (!var->name)
	{
		free(var);
		return (NULL);
	}
	var->value = NULL;
	var->exportstr = NULL;
	var->attributes = 0;
	var->context = 0;
	return (var);
}
