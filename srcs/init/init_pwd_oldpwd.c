#include <stdbool.h>

#include "variables.h"
#include "minishell.h"
#include "general.h"
#include "wrappers.h" // memory

static bool				should_use_imported_pwd(t_shell_var *pwd_var);
static enum e_status	bind_pwd_var(char *current_directory, t_globals *g);

enum e_status	init_pwd(t_globals *g)
{
	enum e_status	status;
	t_shell_var		*pwd_var;
	char			*current_directory;

	pwd_var = find_variable("PWD", g);
	if (should_use_imported_pwd(pwd_var))
	{
		current_directory = canonicalize_path(pwd_var->value, &status);
		if (status == E_MALLOC)
			return (E_MALLOC);
		if (current_directory)
			set_cwd(current_directory, g);
		else
			current_directory = update_cwd("shell_init", g);
		if (g->posix && current_directory)
			return (bind_pwd_var(current_directory, g));
	}
	else
	{
		current_directory = update_cwd("shell-init", g);
		if (current_directory)
			return (bind_pwd_var(current_directory, g));
	}
	return (SUCCESS);
}

enum e_status	init_oldpwd(t_globals *g)
{
	t_shell_var	*oldpwd_var;

	oldpwd_var = find_variable("OLDPWD", g);
	if (!oldpwd_var || !oldpwd_var->value || !is_directory(oldpwd_var->value))
	{
		oldpwd_var = bind_variable("OLDPWD", NULL, 0, g);
		if (!oldpwd_var)
			return (E_MALLOC);
		oldpwd_var->attributes |= (ATT_EXPORTED | ATT_INVISIBLE);
	}
	return (SUCCESS);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static bool	should_use_imported_pwd(t_shell_var	*pwd_var)
{
	if (!pwd_var || !pwd_var->value)
		return (false);
	if (!is_imported_var(pwd_var))
		return (false);
	if (pwd_var->value[0] != '/')
		return (false);
	if (!is_same_file(pwd_var->value, "."))
		return (false);
	return (true);
}

static enum e_status	bind_pwd_var(char *current_directory, t_globals *g)
{
	t_shell_var	*pwd_var;

	pwd_var = bind_variable("PWD", current_directory, 0, g);
	if (!pwd_var)
		return (E_MALLOC);
	pwd_var->attributes |= ATT_EXPORTED;
	g->array_needs_making = true;
	return (SUCCESS);
}
