#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "minishell.h"
#include "general.h"
#include "wrappers.h"
#include "command.h"
#include "variables.h"
#include "redirection.h"

#define DUP_ERR_FORMAT "minishell: redirection error: cannot duplicate fd: \
%s\n"

static enum e_status	add_undo_redirect(int fd, t_globals *g);
static enum e_status	add_undo_close_redirect(int fd, t_globals *g);
static t_redirect		*make_dup_and_close_pair(int fd, int fd_dup);

int	backup_fd(int io_fd, int file_fd, t_globals *g)
{
	enum e_status	status;

	if (file_fd != io_fd && is_valid_fd(io_fd))
		status = add_undo_redirect (io_fd, g);
	else
		status = add_undo_close_redirect(io_fd, g);
	if (status != SUCCESS)
	{
		if (file_fd >= 0)
			close(file_fd);
		if (status == E_MALLOC)
		{
			g->last_command_exit_value = 2; /* XXX - change to macro */
			return (MALLOC_REDIRECT_ERROR);
		}
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

static enum e_status	add_undo_redirect(int fd, t_globals *g)
{
	int			fd_dup;
	t_redirect	*dup_and_close;

	fd_dup = xdup(fd);
	if (fd_dup < 0)
	{
		ft_dprintf(STDERR_FILENO, DUP_ERR_FORMAT, strerror(errno));
		return (E_DUP);
	}
	dup_and_close = make_dup_and_close_pair(fd, fd_dup);
	if (!dup_and_close)
	{
		close(fd_dup);
		return (E_MALLOC);
	}
	dup_and_close->next->next = g->redirection_undo_list;
	g->redirection_undo_list = dup_and_close;
	ioctl(fd_dup, FIOCLEX, 0);
	return (SUCCESS);
}

static enum e_status	add_undo_close_redirect(int fd, t_globals *g)
{
	t_redir_target	dest;
	t_redir_target	source;
	t_redirect		*closer;

	if (!g)
		return (E_VALUE);
	source.fd = fd;
	dest.fd = 0;
	closer = make_redirection(source, R_CLOSE_THIS, dest);
	if (!closer)
		return (E_MALLOC);
	closer->next = g->redirection_undo_list;
	g->redirection_undo_list = closer;
	return (SUCCESS);
}
	// closer->open_flags |= RX_INTERNAL;

static t_redirect	*make_dup_and_close_pair(int fd, int fd_dup)
{
	t_redirect		*duper;
	t_redirect		*closer;
	t_redir_target	source;
	t_redir_target	dest;

	source.fd = fd;
	dest.fd = fd_dup;
	if (fd == 0)
		duper = make_redirection(source, R_DUPLICATING_INPUT, dest);
	else
		duper = make_redirection(source, R_DUPLICATING_OUTPUT, dest);
	if (!duper)
		return (NULL);
	source.fd = fd_dup;
	dest.fd = 0;
	closer = make_redirection(source, R_CLOSE_THIS, dest);
	if (!closer)
	{
		destroy_redirects(duper);
		return (NULL);
	}
	duper->next = closer;
	return (duper);
}
	// closer->open_flags |= RX_INTERNAL;
	// duper->open_flags |= (RX_INTERNAL | RX_SAVEFD);

/* **************************************************************** */
/*	                 Static Functions Documentation	            	*/
/* **************************************************************** */

/**
	@fn enum e_status	add_undo_redirect(int fd, t_globals *g)

	@brief Adds to \ar g->redirection_undo_list a list of redirections
	that will restore \ar fd after it has been redirected.

	@param fd The file descriptor that will be redirected.
	@param g Pointer to the global variables struct.

	@return Execution status:
	@li \ar SUCCESS
	@li \ar E_DUP dup(2) error
	@li \ar E_MALLOC  memory allocation failure

	@diffbash
	@li Doesn't take an \ar fdbase argument. It is only relevant to
	do_redirection_internal() for r_duplicating_input, r_duplicating_output,
	r_move_input or r_move_output when RX_ACTIVE and RX_UNDOABLE are both true.

	@li RX_UNDOABLE is only set by execute_command_internal(),
	execute_null_command() or execute_builtin_or_function, and the
	previously mentioned redirections are not relevant in those contexts,
	as they're not part of the minishell redirections.

	@li r_duplicating_input and r_duplicating_output are only used to undo
	redirections. r_move_input and r_move_output are never used.

	@li Doesn't take into consideration \ar SHELL_FD_BASE. Just duplicates
	to the lowest available file descriptor.

	@showrefby
*/

/**
	@fn enum e_status	add_undo_close_redirect(int fd, t_globals *g)

	@brief Adds a redirect to close a file descriptor to the head of
	\ar g->redirection_undo_list.

	@li Sets the \ar RX_INTERNAL flag on the new redirect.

	@param fd The file descriptor to be closed.
	@param g Pointer to the global variables struct.

	@return Execution status:
		@li \ar SUCCESS
		@li \ar E_VALUE \ar g is NULL.
	    @li \ar E_MALLOC memory allocation failure.

	@showrefby
*/

/**
	@fn static t_redirect	*make_dup_and_close_pair(int fd, int fd_dup)

	@brief Creates the list of redirections to restore \ar fd.

	@li The generated list contains 2 operations. An operation that restores
	\ar fd back to \ar fd_dup, followed by an operation to close \ar fd_dup.

	@param fd The file descriptor that will be redirected.
	@param fd_dup Duplicate of \ar fd.

	@return The redirection list containing the necessary operations,
	or NULL on allocation failure.

	@note The following bits are set in open_flags:
	@li duplication: \ar RX_INTERNAL | \ar RX_SAVEFD
	@li closing: \ar RX_INTERNAL
	
	@todo Figure out if flags are necessary in do_redirection_internal().

	@showrefby
*/
