#include <stddef.h>
#include <stdlib.h>

#include "minishell.h"
#include "jobs.h"
#include "variables.h"
#include "redirection.h"
#include "execute.h"
#include "expansion.h"
#include "builtins.h"
#include "general.h"

typedef int	t_builtin_func(t_word_list *list, t_globals *g);

t_builtin_func *find_shell_builtin(const char *name)
{
    if (!name)
        return (NULL);
    if (ft_strcmp(name, "cd") == 0)
        return (cd_builtin);
    if (ft_strcmp(name, "echo") == 0)
        return (echo_builtin);
    if (ft_strcmp(name, "exit") == 0)
        return (exit_builtin);
    if (ft_strcmp(name, "export") == 0)
        return (export_builtin);
    if (ft_strcmp(name, "pwd") == 0)
        return (pwd_builtin);
    if (ft_strcmp(name, "unset") == 0)
        return (unset_builtin);
    return (NULL);
}

int	execute_null_command(t_redirect *redirects, t_pipes pipes, t_globals *g)
{
	int	redir_status;

	if (pipes.in != NO_PIPE || pipes.out != NO_PIPE)
	{
		if (make_child(g) == 0)
		{
			// restore_original_signals ();
			do_piping(pipes);
			g->interactive = 0;
			redir_status = do_redirections(redirects, 0, g);
			if (redir_status == MALLOC_REDIRECT_ERROR)
				exit(EXECUTION_MALLOC_FAILURE);
			else if (redir_status != SUCCESS)
				exit(EXECUTION_FAILURE);
			exit(EXECUTION_SUCCESS);
		}
		else
		{
			close_pipes(pipes);
			return (EXECUTION_SUCCESS);
		}
	}
	else
	{
		redir_status = do_redirections(redirects, RX_UNDOABLE, g);
		// cleanup_redirects (redirection_undo_list);
		// redirection_undo_list = (REDIRECT *)NULL;
		if (redir_status == MALLOC_REDIRECT_ERROR)
			return (EXECUTION_MALLOC_FAILURE);
		else if (redir_status != SUCCESS)
			return (EXECUTION_FAILURE);
		return (EXECUTION_SUCCESS);
	}
}

void	execute_builtin_in_subshell(t_simple_command *command, t_pipes pipes, char *fds_to_close, t_globals *g)
{
	int	result;
	t_builtin_func *builtin_func;

	builtin_func = find_shell_builtin(command->words->word->word);
	g->interactive = false;
	// without_job_control ();
	g->last_made_pid = NO_PID;
	g->already_making_children = false;
	// set_sigint_handler ();
	close_fd_bitmap(fds_to_close);
	do_piping(pipes);
	if (do_redirections(command->redirects, 0, g) != SUCCESS)
		exit(EXECUTION_FAILURE);
	result = builtin_func(command->words->next, g);
	exit(result);
}

int	execute_builtin(t_simple_command *command, t_globals *g)
{
	int				result;
	t_builtin_func	*builtin_func;

	builtin_func = find_shell_builtin(command->words->word->word);
	if (do_redirections(command->redirects, RX_UNDOABLE, g) != 0)
	{
		do_redirections(g->redirection_undo_list, 0, g);
		g->redirection_undo_list = NULL;
		return (EXECUTION_FAILURE);
	}
	result = builtin_func(command->words->next, g);
	do_redirections(g->redirection_undo_list, 0, g);
	g->redirection_undo_list = NULL;
	return (result);
}

int	execute_simple_command(t_simple_command *command, t_pipes pipes, char *fds_to_close, t_globals *g)
{
	int					result;
	t_simple_command	expanded_command;
	bool				already_forked;
	pid_t				pid;
	enum e_status		status;

	result = EXECUTION_SUCCESS;
	status = SUCCESS;
	expanded_command.flags = 0;
	expanded_command.words = NULL;
	expanded_command.redirects = command->redirects;
	already_forked = false;
	if (pipes.in != NO_PIPE || pipes.out != NO_PIPE)
	{
		pid = make_child(g);
		if (pid == 0)
		{
			already_forked = true;
			close_fd_bitmap(fds_to_close);
			do_piping(pipes);
			pipes.in = NO_PIPE;
			pipes.out = NO_PIPE;
		}
		else
		{
			if (pipes.out != NO_PIPE)
				result = g->last_command_exit_value;
			close_pipes(pipes);
			return (result);
		}
	}
	expanded_command.words = expand_word_list(command->words, &status, g);
	if (!expanded_command.words)
	{
		result = execute_null_command(command->redirects, pipes, g);
		if (already_forked)
			exit(result);
		else
		{
			g->last_command_exit_value = result;
			return (result);
		}
	}
	if (find_shell_builtin(expanded_command.words->word->word) != NULL)
	{
		if (already_forked)
		{
			// reset_signal_handlers ();
			execute_builtin_in_subshell(&expanded_command, pipes, fds_to_close, g);
		}
		else
		{
			result = execute_builtin(&expanded_command, g);
		}
	}
	else
		result = execute_disk_command(&expanded_command, pipes, fds_to_close, g);
	destroy_word_list(expanded_command.words);
	return (result);
}

// /* The meaty part of all the executions.  We have to start hacking the
//    real execution of commands here.  Fork a process, set things up,
//    execute the command. */
// static int
// execute_simple_command (simple_command, pipe_in, pipe_out, async, fds_to_close)
//      SIMPLE_COM *simple_command;
//      int pipe_in, pipe_out, async;
//      struct fd_bitmap *fds_to_close;
// {
//   WORD_LIST *words, *lastword;
//   char *command_line, *lastarg, *temp;
//   int first_word_quoted, result, builtin_is_special, already_forked, dofork;
//   int fork_flags, cmdflags;
//   pid_t old_last_async_pid;
//   sh_builtin_func_t *builtin;
//   SHELL_VAR *func;
//   volatile int old_builtin, old_command_builtin;

//   result = EXECUTION_SUCCESS;
//   cmdflags = simple_command->flags;
//   already_forked = 0;
//   dofork = pipe_in != NO_PIPE || pipe_out != NO_PIPE;

//   if (dofork)
//     {
//     if (make_child (p = savestring (the_printed_command_except_trap), fork_flags) == 0)
// 	{
// 		already_forked = 1;
// 		if (fds_to_close)
// 			close_fd_bitmap (fds_to_close);
// 		do_piping (pipe_in, pipe_out);
// 		pipe_in = pipe_out = NO_PIPE;
// 	}
//       else
// 	{

// 	  if (pipe_out != NO_PIPE)
// 	    result = last_command_exit_value;
// 	  close_pipes (pipe_in, pipe_out);  /* don't free this. */
// 	  return (result);
// 	}
//     }

//   QUIT;		/* XXX */

//   /* If we are re-running this as the result of executing the `command'
//      builtin, do not expand the command words a second time. */

//       words = expand_words (simple_command->words);

//   /* It is possible for WORDS not to have anything left in it.
//      Perhaps all the words consisted of `$foo', and there was
//      no variable `$foo'. */
//   if (words == 0)
//     {
//       this_command_name = 0;
//       result = execute_null_command (simple_command->redirects,
// 				     pipe_in, pipe_out,
// 				     already_forked ? 0 : async);
//       if (already_forked)
// 	sh_exit (result);
//       else
// 	{
// 	  bind_lastarg ((char *)NULL);
// 	  set_pipestatus_from_exit (result);
// 	  return (result);
// 	}
//     }
//     builtin = find_shell_builtin (this_command_name);

//   last_shell_builtin = this_shell_builtin;
//   this_shell_builtin = builtin;

//   if (builtin || func)
//     {
//       if (already_forked)
// 	{
// 	//   reset_signal_handlers ();

// 	  if (async == 0)
// 	    subshell_level++;
// 	  execute_builtin_in_subshell_or_function(words, simple_command->redirects, builtin, func,
// 	     pipe_in, pipe_out, async, fds_to_close,
// 	     cmdflags);
// 	  subshell_level--;
// 	}
//       else
// 	{
// 	  result = execute_builtin_or_function(words, builtin, func, simple_command->redirects, fds_to_close,
// 	     cmdflags);
// 	  if (builtin)
// 	    {
// 	      if (result > EX_SHERRBASE)
// 		{
// 		  switch (result)
// 		    {
// 		    case EX_REDIRFAIL:
// 		    case EX_BADASSIGN:
// 		    case EX_EXPFAIL:
// 		      /* These errors cause non-interactive posix mode shells to exit */
// 		      if (posixly_correct && builtin_is_special && interactive_shell == 0)
// 			{
// 				printf("posixly_correct && builtin_is_special && interactive_shell == 0\n");
// 			  last_command_exit_value = EXECUTION_FAILURE;
// 			  jump_to_top_level (ERREXIT);
// 			}
// 		      break;
// 		    case EX_DISKFALLBACK:
// 		      /* XXX - experimental */
// 		      executing_builtin = old_builtin;
// 		      executing_command_builtin = old_command_builtin;
// 		      builtin = 0;

// 		      /* The redirections have already been `undone', so this
// 			 will have to do them again. But piping is forever. */
// 		      pipe_in = pipe_out = -1;
// 		      goto execute_from_filesystem;
// 		    }
// 		  result = builtin_status (result);
// 		  if (builtin_is_special)
// 		    special_builtin_failed = 1;	/* XXX - take command builtin into account? */
// 		}
// 	      /* In POSIX mode, if there are assignment statements preceding
// 		 a special builtin, they persist after the builtin
// 		 completes. */
// 	      if (posixly_correct && builtin_is_special && temporary_env)
// 		merge_temporary_env ();
// 	    }
// 	  else		/* function */
// 	    {
// 	      if (result == EX_USAGE)
// 		result = EX_BADUSAGE;
// 	      else if (result > EX_SHERRBASE)
// 		result = builtin_status (result);
// 	    }

// 	  set_pipestatus_from_exit (result);

// 	  goto return_result;
// 	}
//     }
//   result = execute_disk_command (words, simple_command->redirects, command_line,
// 			pipe_in, pipe_out, async, fds_to_close,
// 			cmdflags);

//   dispose_words (words);
//   return (result);
// }