#include <stddef.h>

#include "execute.h"
#include "general.h"
#include "variables.h"

/*
	Sets all Bash global variables to initial values or 0 if uninitialized.
*/
void	init_global_variables(t_globals *g)
{
	g->posix = 0;
	g->last_command_exit_value = 0;
	g->current_working_directory = (char *) NULL;
	g->shell_level = 0;
	g->really_exit = false;
	g->got_tty_state = false;
	g->last_made_pid = NO_PID;
	g->pid_list = NULL;
	g->export_env = (char **) NULL;
	g->export_env_size = 0;
	g->export_env_index = 0;
	g->array_needs_making = true;
	g->global_variables = (t_var_context *) NULL;
	g->shell_variables = (t_var_context *) NULL;
	g->variable_context = 0;
	g->temporary_env = (t_hash_table *) NULL;
	g->hashed_filenames = (t_hash_table *) NULL;
	g->last_table_searched = (t_hash_table *) NULL;
	g->table_to_bind_to = NULL;
	g->ifs_var = 0;
	g->ifs_value = 0;
	ft_memset(g->ifs_cmap, '\0', sizeof(g->ifs_cmap));
	g->ifs_is_set = false;
	g->ifs_is_null = false;
	g->expanding_redir = 0;
	g->executing_builtin = 0;
	g->subshell_environment = 0;
	g->ifs_firstc = '\0';
	g->heredoc_errno = 0;
	g->redirection_undo_list = NULL;
}
