#include <stddef.h>

#include "variables.h"

static t_shell_var	*find_variable_internal(const char *name, int flags, \
											t_globals *g);

t_shell_var	*find_variable(const char *name, t_globals *g)
{
	int			flags;

	if (!name || !g)
		return (NULL);
	flags = 0;
	g->last_table_searched = NULL;
	if (!g->expanding_redir && g->executing_builtin)
		flags |= FV_FORCETEMPENV;
	return (find_variable_internal(name, flags, g));
}

t_shell_var	*find_variable_tempenv(const char *name, t_globals *g)
{
	if (!name || !g)
		return (NULL);
	return (find_variable_internal(name, FV_FORCETEMPENV, g));
}

t_shell_var	*find_variable_notempenv(const char *name, t_globals *g)
{
	if (!name || !g)
		return (NULL);
	return (find_variable_internal(name, 0, g));
}

// SHELL_VAR *
// find_global_variable (name)
//      const char *name;
// {
//   SHELL_VAR *var;

//   var = var_lookup (name, global_variables);
//   if (var && nameref_p (var))
//     var = find_variable_nameref (var);

//   if (var == 0)
//     return ((SHELL_VAR *)NULL);

//   return (var->dynamic_value ? (*(var->dynamic_value)) (var) : var);
// }

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static t_shell_var	*find_variable_internal(const char *name, int flags, \
											t_globals *g)
{
	t_shell_var		*var;
	const bool		force_tempenv = (flags & FV_FORCETEMPENV);
	const bool		search_tempenv = (force_tempenv \
										|| (g && !g->expanding_redir \
										&& g->subshell_environment));

	if (!name || !g)
		return (NULL);
	var = NULL;
	if (search_tempenv && g->temporary_env)
		var = hash_lookup(name, g->temporary_env, g);
	if (!var)
		var = var_lookup(name, g->shell_variables, g);
	return (var);
}

/* **************************************************************** */
/*	                 Static Functions Documentation	            	*/
/* **************************************************************** */

/**
	@fn static t_shell_var	*find_variable_internal(const char *name, 
											int flags, t_globals *g)

	@brief Workhorse for find_variable(), find_variable_tempenv() and
	find_variable_tempenv().

	If \ar FV_FORCETEMPENV is set in \ar flags or if executing a builtin
	or in a subshell, searches in g->temporary_env before searching
	in g->shell_variables.

	@param name The name of the variable to find.

	@diffbash
	@li Doesn't handle dynamic variables.
	@li Doesn't handle invisible variables (only function to ever set
		FV_SKIPINVISIBLE flag is find_variable_no_invisible() and this function
		is never called anywhere...)

	@showrefs
	@showrefby
*/