#include "minishell.h"
#include "input.h"
#include "init.h"
#include "sig.h"
#include "wrappers.h" // memory

volatile sig_atomic_t	g_signal = NO_SIGNAL;

void	deinit_minishell(t_globals *g);
void	print_ast(t_command *command, int level);
int		read_and_execute_loop(t_input *input, t_globals *g);

int	main(int argc, char **argv, char **envp)
{
	int			exit_status;
	t_input		input;
	t_globals	global_variables;

	init_minishell(argc, envp, &input, &global_variables);
	exit_status = read_and_execute_loop(&input, &global_variables);
	deinit_minishell(&global_variables);
	return (exit_status);
}

void	deinit_minishell(t_globals *g)
{
	free(g->current_working_directory);
	destroy_var_context(g->shell_variables);
}

// while [ 1 ]; do leaks -q minishell; sleep 1; done
// __attribute__ ((destructor)) void	destructor(void)
// {
// 	system("leaks -q minishell");
// }
