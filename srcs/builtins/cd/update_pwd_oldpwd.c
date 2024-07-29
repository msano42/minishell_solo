#include "minishell.h"
#include "variables.h"
#include "general.h"
#include "wrappers.h" // memory

/**
	@fn enum e_status	update_pwd_oldpwd(t_globals *g)

	@brief Updates the \ar PWD and \ar OLDPWD variables.

	@li If \ar g->current_working_directory is NULL, it is
	updated first using get_working_directory().

	@li \ar OLDPWD is set to the value of \ar PWD.

	@li \ar PWD is set to the value of
	\ar g->current_working_directory or "" if it is NULL.

	@param g Pointer to the global variables struct.

	@return Execution status:
	@li \ar SUCCESS
	@li \ar E_MALLOC - memory allocation failure

	@seebash @verbatim bindpwd() @endverbatim
	@diffbash
	@li Doesn't take a \ar no_symlinks argument.
	@li Doesn't optimize with update_export_env_inplace().

	@showrefby
*/
enum e_status	update_pwd_oldpwd(t_globals *g)
{
	char			*pwd_value;
	char			*oldpwd_value;

	if (!g->current_working_directory)
		update_cwd("cd", g);
	oldpwd_value = get_string_value("PWD", g);
	if (!bind_variable("OLDPWD", oldpwd_value, 0, g))
		return (E_MALLOC);
	pwd_value = g->current_working_directory;
	if (!pwd_value)
		pwd_value = "";
	if (!bind_variable("PWD", pwd_value, 0, g))
		return (E_MALLOC);
	return (SUCCESS);
}
