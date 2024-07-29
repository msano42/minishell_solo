#include <stddef.h>
#include <string.h>
#include <errno.h>

#include "wrappers.h" // memory
#include "minishell.h"
#include "variables.h"
#include "redirection.h"
#include "general.h"
#include "redirection_internal.h"

#define AMBIGUOUS_REDIR_ERR "minishell: %s: ambiguous redirect\n"
#define HEREDOC_REDIR_ERR "minishell: cannot create redirect file for \
here-document: %s\n"
#define DEFAULT_REDIR_ERR "minishell: %s: %s\n"

static int	do_single_redirection(t_redirect *redirect, int flags, \
			char **filename_ptr, t_globals *g);
static void	redirection_error(t_redirect *redirect, int error, \
			char *filename, int heredoc_errno);
static void	print_redirection_error(int error, const char *error_str, \
			int heredoc_errno);

int	do_redirections(t_redirect *list, uint8_t flags, t_globals *g)
{
	int			error;
	char		*filename;

	if (flags & RX_UNDOABLE)
	{
		destroy_redirects(g->redirection_undo_list);
		g->redirection_undo_list = NULL;
	}
	while (list)
	{
		filename = NULL;
		error = do_single_redirection(list, flags, &filename, g);
		if (error)
		{
			if (error != MALLOC_REDIRECT_ERROR)
				redirection_error(list, error, filename, g->heredoc_errno);
			free(filename);
			return (error);
		}
		free(filename);
		list = list->next;
	}
	return (SUCCESS);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static int	do_single_redirection(t_redirect *redirect, int flags, \
			char **fnp, t_globals *g)
{
	int	status;

	status = SUCCESS;
	if (is_io_redirection(redirect->instruction))
		status = do_io_redirection(redirect, flags, fnp, g);
	else if (redirect->instruction == R_READING_UNTIL)
		status = do_heredoc_redirection(redirect, flags, g);
	else if (is_dup_redirection(redirect->instruction))
		status = do_dup_redirection(redirect);
	else if (redirect->instruction == R_CLOSE_THIS)
		status = do_close_redirection(redirect, g);
	return (status);
}

/*
	Added check for R_READING_UNTIL. Bash seems to bug by
	trying to print redirectee.fd (actually a char *)...
*/
static void	redirection_error(t_redirect *redirect, int error, \
			char *filename, int heredoc_errno)
{
	char	fd_buf[12];

	if (error == EBADF)
	{
		if (is_dup_redirection(redirect->instruction))
			ft_itoa_no_alloc(redirect->redirectee.fd, fd_buf);
		else
			ft_itoa_no_alloc(redirect->redirector.fd, fd_buf);
		filename = fd_buf;
	}
	else if (!filename)
	{
		if (is_expandable_filename_redirection(redirect))
			filename = redirect->redirectee.filename->word;
		else
		{
			if (redirect->instruction == R_READING_UNTIL)
				ft_itoa_no_alloc(redirect->redirector.fd, fd_buf);
			else
				ft_itoa_no_alloc(redirect->redirectee.fd, fd_buf);
			filename = fd_buf;
		}
	}
	print_redirection_error(error, filename, heredoc_errno);
}

static void	print_redirection_error(int error, const char *err_str, \
			int heredoc_errno)
{
	if (error == AMBIGUOUS_REDIRECT_ERROR)
		ft_dprintf(STDERR_FILENO, AMBIGUOUS_REDIR_ERR, err_str);
	else if (error == HEREDOC_REDIRECT_ERROR)
		ft_dprintf(STDERR_FILENO, HEREDOC_REDIR_ERR, strerror(heredoc_errno));
	else
		ft_dprintf(STDERR_FILENO, DEFAULT_REDIR_ERR, err_str, strerror(error));
}
