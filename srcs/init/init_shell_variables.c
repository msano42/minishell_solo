#include <stddef.h>

#include "minishell.h"
#include "variables.h"
#include "init_internal.h"

#define DEFAULT_PATH_VALUE "/usr/local/bin:/usr/local/sbin:/usr/bin:\
/usr/sbin:/bin:/sbin:."

/*
	Imports environment variables and handles some variables specially set in
	Bash initialize_shell_variables(). (PWD, OLD_PWD, PATH, IFS, SHLVL)

	Unlike Bash, does not set dollar_dollar_pid ($$), TERM, NODE, PS1, PS2,
	HOSTTYPE, OSTYPE, MACHTYPE, HOSTNAME, MAILCHECK, PPID, BASH_ARGV0, OPTIND,
	OPTERR, HOME, BASH, SHELL, BASH_VERSION, BASH_EXECUTION_STRING,
	POSIXLY_CORRECT, POSIX_PEDANTIC, HISTFILE, IGNOREEOF, ignoreeof,
	HISTCONTROL, HISTIGNORE, HISTTIMEFORMAT, UID, EUID, BASH_LOADABLES_PATH,
	BASH_XTRACEFD unless imported from the environment.
*/
enum e_status	init_shell_variables(char **envp, t_globals *g)
{
	t_shell_var		*ifs_var;

	if (create_variable_table(g) == E_MALLOC)
		return (E_MALLOC);
	if (import_environment(envp, g) == E_MALLOC)
		return (E_MALLOC);
	if (init_pwd(g) == E_MALLOC)
		return (E_MALLOC);
	if (init_oldpwd(g) == E_MALLOC)
		return (E_MALLOC);
	if (set_if_not("PATH", DEFAULT_PATH_VALUE, g) == NULL)
		return (E_MALLOC);
	ifs_var = bind_variable("IFS", " \t\n", 0, g);
	if (!ifs_var)
		return (E_MALLOC);
	set_ifs(ifs_var, g);
	if (adjust_shell_level(1, g) == E_MALLOC)
		return (E_MALLOC);
	return (SUCCESS);
}
