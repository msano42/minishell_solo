#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "builtins.h"
#include "redirection.h"
#include "expansion.h"
#include "general.h"

#include <unistd.h>
#include <sys/wait.h>

extern char **global_envp;

char	**make_argv(t_word_list *list);
char	**make_envp(t_hash_table *table);
char	*search_for_command(const char *path_name, enum e_status *status, t_globals *g);

static bool	is_builtin(char *str);
static int	execute_disk(t_word_list *words, t_redirect *redirects, t_globals *g);

void	simple_exec(t_command *command, t_globals *g)
{
	char		*builtin;
	t_word_list	*args;
	t_word_list	*words;
	enum e_status	status;

	status = SUCCESS;
	if (!command || command->type != CM_SIMPLE)
		return ;

	words = expand_word_list(command->u_value.simple->words, &status, g);
	command->u_value.simple->words = NULL;
	builtin = words->word->word;
	if (!is_builtin(builtin))
	{
		g->last_command_exit_value = execute_disk(words, command->u_value.simple->redirects, g);
		destroy_word_list(words);
		return ;
	}
	args = words->next;
	if (do_redirections(command->u_value.simple->redirects, RX_UNDOABLE, g))
	{
		g->last_command_exit_value = EXECUTION_FAILURE;
		do_redirections(g->redirection_undo_list, 0, g);
		destroy_redirects(g->redirection_undo_list);
		g->redirection_undo_list = NULL;
		destroy_word_list(words);
		return ;
	}
	if (!ft_strcmp(builtin, "cd"))
		g->last_command_exit_value = cd_builtin(args, g);
	else if (!ft_strcmp(builtin, "echo"))
		g->last_command_exit_value = echo_builtin(args, g);
	else if (!ft_strcmp(builtin, "exit"))
		g->last_command_exit_value = exit_builtin(args, g);
	else if (!ft_strcmp(builtin, "export"))
		g->last_command_exit_value = export_builtin(args, g);
	else if (!ft_strcmp(builtin, "pwd"))
		g->last_command_exit_value = pwd_builtin(args, g);
	else if (!ft_strcmp(builtin, "unset"))
		g->last_command_exit_value = unset_builtin(args, g);
	do_redirections(g->redirection_undo_list, 0, g);
	destroy_redirects(g->redirection_undo_list);
	destroy_word_list(words);
	g->redirection_undo_list = NULL;
}

static int	execute_disk(t_word_list *words, t_redirect *redirects, t_globals *g)
{
	pid_t	pid;
	enum e_status status;
	char	**argv;
	char	**envp;

	pid = fork();
	if (pid == 0)
	{
		envp = make_envp(g->shell_variables->table);
		argv = make_argv(words);
		if (do_redirections(redirects, 0, g))
			exit(EXECUTION_FAILURE);
		execve(search_for_command(words->word->word, &status, g), argv, envp);
		exit(0);
	}
	int	stat;

	waitpid(pid, &stat, 0);
	return (WEXITSTATUS(stat));
}

static bool	is_builtin(char *str)
{
	if (!ft_strcmp(str, "cd"))
		return (true);
	else if (!ft_strcmp(str, "echo"))
		return (true);
	else if (!ft_strcmp(str, "exit"))
		return (true);
	else if (!ft_strcmp(str, "export"))
		return (true);
	else if (!ft_strcmp(str, "pwd"))
		return (true);
	else if (!ft_strcmp(str, "unset"))
		return (true);
	else if (!ft_strcmp(str, "print"))
		return (true);
	return (false);
}

