#include "minishell.h"
#include "parser.h"
#include "variables.h"
#include "init_internal.h"

#include <stdlib.h> // exit()
#include <unistd.h> // isatty()

/*
	Does initialization done in Bash main() relevant to the minishell and
	initializes Bash global variables.

	Unlike Bash:
		- Does not accept command line arguments.
		- Exits if non-interactive. (might consider handling non-interactive)
		- Assumes login_shell, privileged_mode, posixly_correct are all 0.
		- Doesn't get user ids. (getgid, geteuid, getegid are forbidden)
		- Doesn't set shell_name or shell_start_time.
		- Doesn't check_dev_tty(). (seems useless??)
		- Doesn't handle locale.
		- Ignores a bunch of other complicated stuff.
*/
enum e_status	init_minishell(int argc, char **envp, \
								t_input *input, t_globals *g)
{
	if (argc != 1)
	{
		printf("minishell: too many arguments\n"); // placeholder
		exit(2);
	}
	init_global_variables(g);
	init_input(input, g);
	if (init_shell_variables(envp, g) == E_MALLOC)
		exit(2); // placeholder
	return (SUCCESS);
}
