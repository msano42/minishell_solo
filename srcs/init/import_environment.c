#include <stddef.h>

#include "minishell.h"
#include "general.h"
#include "variables.h"

static enum e_status	import_env_string(char *name, t_globals *g);

/*
	If envp is not NULL, imports environment variables from envp.

	Returns E_MALLOC on allocation failure, otherwise returns SUCESS
	(even if envp is NULL).

	NB: This is the first for() loop in Bash initialize_shell_variables().
*/
enum e_status	import_environment(char **envp, t_globals *g)
{
	size_t	i;

	if (!envp)
		return (SUCCESS);
	i = 0;
	while (envp[i])
	{
		if (import_env_string(envp[i], g) == E_MALLOC)
			return (E_MALLOC);
		i++;
	}
	return (SUCCESS);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

/*
	Adds a variable to the shell_variables/global_variables context.

	Does not import the variable if the name is not a legal identifier of if
	str is in the following formats:
		- "=xxx"
		- "xxx"
		- "="
		- ""
	(Accepts "xxx=" though.)

	Unlike Bash, doesn't import illegal names into invalid_env.

	NB: This function is weird but this is how Bash does it.
*/
static enum e_status	import_env_string(char *str, t_globals *g)
{
	char			c;
	size_t			i;
	t_shell_var		*var;
	char			*value;

	value = str;
	c = *value++;
	while (c && c != '=')
		c = *value++;
	i = 0;
	if (value[-1] == '=')
		i = value - str - 1;
	str[i] = '\0';
	if (i && is_legal_identifier(str))
	{
		var = bind_variable(str, value, 0, g);
		str[i] = '=';
		if (!var)
			return (E_MALLOC);
		var->attributes |= (ATT_EXPORTED | ATT_IMPORTED);
		g->array_needs_making = true;
	}
	str[i] = '=';
	return (SUCCESS);
}
