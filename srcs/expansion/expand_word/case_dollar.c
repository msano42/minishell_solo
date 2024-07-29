#include "expansion.h"
#include "general.h"
#include "variables.h"
#include "wrappers.h" // memory

static char	*get_name_for_variable_expansion(const char *str, \
			t_expansion *exp);
static char	*expand_variable(const char *name, t_expansion *exp, t_globals *g);

void	case_dollar(const char *str, t_expansion *exp, t_globals *g)
{
	char	*name;
	char	*parameter;

	if (str[++exp->idx] == '?')
	{
		parameter = ft_itoa(g->last_command_exit_value);
		if (!parameter)
		{
			exp->status = E_MALLOC;
			return ;
		}
		exp->idx++;
	}
	else
	{
		name = get_name_for_variable_expansion(str, exp);
		if (exp->status == E_MALLOC)
			return ;
		parameter = expand_variable(name, exp, g);
		free(name);
		if (exp->status == E_MALLOC)
			return ;
	}
	add_string_to_expansion(parameter, exp);
	free(parameter);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static char	*get_name_for_variable_expansion(const char *str, t_expansion *exp)
{
	char	*name;
	size_t	start;

	start = exp->idx;
	while (str[exp->idx] && is_legal_variable_char(str[exp->idx]))
		exp->idx++;
	if (exp->idx == start)
	{
		exp->expanded_something = false;
		return (NULL);
	}
	name = substring(str, start, exp->idx);
	if (!name)
		exp->status = E_MALLOC;
	return (name);
}

static char	*expand_variable(const char *name, t_expansion *exp, t_globals *g)
{
	t_shell_var	*var;
	char		*parameter;

	if (!name)
	{
		parameter = ft_strdup("$");
		if (!parameter)
			exp->status = E_MALLOC;
		return (parameter);
	}
	var = find_variable(name, g);
	if (var && !is_invisible_var(var) && var->value)
	{
		if (var->value[0] && in_dquotes_or_heredoc(exp))
			parameter = quote_string(var->value);
		else
			parameter = quote_escapes(var->value, g->ifs_value);
		if (!parameter)
			exp->status = E_MALLOC;
	}
	else
		parameter = NULL;
	return (parameter);
}
