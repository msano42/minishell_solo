#include <stdio.h>

#include "builtins_internal.h"
#include "minishell.h"
#include "general.h"
#include "variables.h"
#include "wrappers.h" // memory

static enum e_status	show_variable_attributes(t_shell_var *var, bool posix);
static bool				is_show_exported_candidate(t_shell_var *var);
static int				compare_shell_variable_names(void *var_1, void *var_2);

int	show_exported_variables(t_globals *g)
{
	size_t		i;
	t_shell_var	*var;
	void		**exported_vars;

	exported_vars = make_variable_array(is_show_exported_candidate, \
					g->shell_variables->table);
	if (!exported_vars)
		return (EXECUTION_MALLOC_FAILURE);
	bubble_sort_ptr_array(exported_vars, compare_shell_variable_names);
	i = 0;
	while (exported_vars[i])
	{
		var = (t_shell_var *)exported_vars[i];
		if (show_variable_attributes(var, g->posix) == E_WRITE)
		{
			free(exported_vars);
			return (EXECUTION_FAILURE);
		}
		i++;
	}
	free(exported_vars);
	return (EXECUTION_SUCCESS);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static bool	is_show_exported_candidate(t_shell_var *var)
{
	if (!var)
		return (false);
	if ((var->attributes & (ATT_INVISIBLE | ATT_IMPORTED)) \
		== (ATT_INVISIBLE | ATT_IMPORTED))
		return (false);
	return (is_exported_var(var));
}

static int	compare_shell_variable_names(void *var_1, void *var_2)
{
	const char	*name_1 = ((t_shell_var *)var_1)->name;
	const char	*name_2 = ((t_shell_var *)var_2)->name;

	return (ft_strcmp(name_1, name_2));
}

static enum e_status	show_variable_attributes(t_shell_var *var, bool posix)
{
	enum e_status	status;
	char			*prefix;
	char			*formatted_value;

	status = SUCCESS;
	prefix = "declare -x ";
	if (posix)
		prefix = "export ";
	if (builtin_putstr(prefix, "export") == E_WRITE)
		return (E_WRITE);
	if (is_invisible_var(var) || !var->value)
	{
		if (builtin_putstr_nl(var->name, "export") == E_WRITE)
			return (E_WRITE);
		return (SUCCESS);
	}
	if (has_unprintable_chars(var->value))
		formatted_value = strfmt_make_printable(var->value);
	else
		formatted_value = strfmt_double_quote_and_escape(var->value);
	if (builtin_put_assign_nl(var->name, formatted_value, "export") == E_WRITE)
		status = E_WRITE;
	free(formatted_value);
	return (status);
}
