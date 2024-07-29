#include "command.h"
#include "execute.h"
#include "redirection.h"
#include "minishell.h"
#include "general.h"
#include "jobs.h"
#include "wrappers.h"
#include "sig.h"

#include <unistd.h>

#define EX_NOTFOUND	127

static void	execute_disk_command_child(char *command, t_word_list *words, t_redirect *redirects, t_globals *g);

#include <stdio.h>
int	execute_disk_command(t_simple_command *simple_command, t_pipes pipes, char *fds_to_close, t_globals *g)
{
	enum e_status	status;
	char			*command;
	pid_t			pid;

	status = SUCCESS;
	command = search_for_command(simple_command->words->word->word, &status, g);
	if (status == E_MALLOC)
		return (EXECUTION_MALLOC_FAILURE);
	pid = make_child(g);
	if (pid == 0)
	{
		init_child_sighandlers();
		// reset_terminating_signals();
		// restore_original_signals();
		close_fd_bitmap(fds_to_close);
		do_piping(pipes);
		execute_disk_command_child(command, simple_command->words, simple_command->redirects, g);
	}
	else
	{
		close_pipes(pipes);
		free(command);
	}
	return (EXECUTION_SUCCESS);
}

// if (command && bind_variable("_", command, 0, g) == NULL)
// 	return (EXECUTION_MALLOC_FAILURE);
// if ((simple_command->flags & CMD_NO_FORK) && pipes.in == NO_PIPE && pipes.out == NO_PIPE)
// 	pid = 0;
// else

static void	execute_disk_command_child(char *command, t_word_list *words, t_redirect *redirects, t_globals *g)
{
	int		redirection_status;
	char	**shell_envp;
	char	**shell_argv;

	redirection_status = do_redirections(redirects, 0, g);
	if (redirection_status == E_MALLOC)
		exit(EXECUTION_MALLOC_FAILURE);
	else if (redirection_status != SUCCESS)
		exit(EXECUTION_FAILURE);
	if (!command)
	{
		command = printable_filename(command);
		ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n", words->word->word);
		free(command);
		exit(EX_NOTFOUND);
	}
	shell_argv = make_argv(words);
	shell_envp = make_envp(g->shell_variables->table);
	// exit(execve(command, shell_argv, shell_envp));
	execve(command, shell_argv, shell_envp);
}
