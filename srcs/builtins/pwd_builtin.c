#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "general.h"
#include "command.h"
#include "builtins.h"
#include "builtins_internal.h"

int	pwd_builtin(t_word_list *list, t_globals *g)
{
	if (check_options(&list, "pwd", PWD_USAGE, PWD_LONGDOC) == EX_BADUSAGE)
		return (EX_BADUSAGE);
	if (!g->current_working_directory)
		update_cwd("pwd", g);
	if (g->posix && !is_same_file(".", g->current_working_directory))
		update_cwd("pwd", g);
	if (!g->current_working_directory)
		return (EXECUTION_FAILURE);
	if (builtin_putstr_nl(g->current_working_directory, "pwd") == E_WRITE)
		return (EXECUTION_FAILURE);
	return (EXECUTION_SUCCESS);
}
