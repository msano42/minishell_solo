#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>

#include "variables.h"
#include "general.h"
#include "command.h"
#include "wrappers.h"
#include "redirection.h"
#include "redirection_internal.h"

static int	redirect_io(int io_fd, int file_fd);

int	do_io_redirection(t_redirect *redirect, int flags, char **fnp, \
t_globals *g)
{
	int			file_fd;
	t_word		*filename;
	int			status;
	const int	io_fd = redirect->redirector.fd;
	const int	old_filename_flags = redirect->redirectee.filename->flags;

	filename = redirect->redirectee.filename;
	if (g->posix && !g->interactive)
		filename->flags |= W_NOGLOB;
	status = filename_expand(filename, fnp, g);
	filename->flags = old_filename_flags;
	if (status == E_MALLOC)
		return (MALLOC_REDIRECT_ERROR);
	if (!*fnp)
		return (AMBIGUOUS_REDIRECT_ERROR);
	file_fd = open(*fnp, redirect->open_flags, 0666);
	if (file_fd < 0)
		return (errno);
	if (flags & RX_UNDOABLE)
	{
		status = backup_fd(io_fd, file_fd, g);
		if (status != SUCCESS)
			return (status);
	}
	return (redirect_io(io_fd, file_fd));
}

int	do_heredoc_redirection(t_redirect *redirect, int flags, t_globals *g)
{
	int				heredoc_fd;
	enum e_status	tempfd_status;
	int				backup_status;

	if (!redirect->redirectee.filename)
		return (SUCCESS);
	heredoc_fd = heredoc_to_fd(redirect->redirectee.filename, \
				&tempfd_status, g);
	if (heredoc_fd < 0)
	{
		if (tempfd_status == E_MALLOC)
			return (MALLOC_REDIRECT_ERROR);
		g->heredoc_errno = errno;
		return (HEREDOC_REDIRECT_ERROR);
	}
	if (flags & RX_UNDOABLE)
	{
		backup_status = backup_fd(STDIN_FILENO, heredoc_fd, g);
		if (backup_status != SUCCESS)
			return (backup_status);
	}
	return (redirect_io(STDIN_FILENO, heredoc_fd));
}

int	do_dup_redirection(t_redirect *redirect)
{
	const int	old_fd = redirect->redirectee.fd;
	const int	new_fd = redirect->redirector.fd;

	if (old_fd != new_fd)
	{
		if (dup2(old_fd, new_fd) < 0)
			return (errno);
	}
	return (SUCCESS);
}

int	do_close_redirection(t_redirect *redirect, t_globals *g)
{
	int	status;

	status = close(redirect->redirector.fd);
	if (status < 0 && (errno == EIO || errno == ENOSPC))
	{
		g->last_command_exit_value = EXECUTION_FAILURE;
		if (!errno)
			return (EINVAL);
		return (errno);
	}
	return (SUCCESS);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

/* Bash only saves errno on dup() error for heredoc not io */
static int	redirect_io(int io_fd, int file_fd)
{
	int	saved_errno;

	if (file_fd != io_fd)
	{
		if (dup2(file_fd, io_fd) < 0)
		{
			saved_errno = errno;
			close(file_fd);
			return (saved_errno);
		}
		close(file_fd);
	}
	return (SUCCESS);
}
