#include "variables.h"
#include "general.h"
#include "wrappers.h" // memory

#include <stdbool.h>
#include <stddef.h>

static bool	is_envp_candidate(t_shell_var *var);
void		free_ptr_array(void **array);

char	**make_envp(t_hash_table *table)
{
	size_t		i;
	t_shell_var	*var;
	void		**array;

	array = make_variable_array(is_envp_candidate, table);
	if (!array)
		return (NULL);
	i = 0;
	while (array[i])
	{
		var = (t_shell_var *)array[i];
		array[i] = join_with_separator(var->name, var->value, '=');
		if (!array[i])
		{
			free_ptr_array(array);
			return (NULL);
		}
		i++;
	}
	array[i] = NULL;
	return ((char **)array);
}

void	free_ptr_array(void **array)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
}

static bool	is_envp_candidate(t_shell_var *var)
{
	if (!var)
		return (false);
	if (!is_exported_var(var))
		return (false);
	return (!is_invisible_var(var) || is_imported_var(var));
}
