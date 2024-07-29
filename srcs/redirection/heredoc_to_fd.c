#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "minishell.h"
#include "general.h"
#include "wrappers.h" // memory
#include "variables.h"
#include "redirection_internal.h"

static int	make_heredoc_fd(char *document, enum e_status *status);
static int	heredoc_write(int fd, char *heredoc);

/**
	@fn int	heredoc_to_fd(t_word *content, enum e_status *status)

	@brief Transcribes \ar content into a file descriptor that can be
	read as standard input.

	@param content Pointer to a t_word with the content of the here-document.
	@param status A pointer to an enum e_status representing the status
	of the operation.

	@return File descriptor for the here document if successful,
	-1 on error.

	\ar *status is set according to the execution status:
	@li \ar SUCCESS
	@li \ar E_ERROR general error (check \ar errno)
	@li \ar E_MALLOC memory allocation failure
	@li \ar E_OPEN: open(2) error
	@li \ar E_READ: read(2) error

	@warning Because of the way make_heredoc_fd() is implemented, in certain
	error cases, \ar *status may be set to the general error value \ar E_ERROR.
	In that case, \ar errno should be used for error handling.

	@seebash @verbatim here_document_to_fd() @endverbatim

	@diffbash
	@li Skips using a pipe regardless of the size of the here-document. Always
	creates a file.
	@li Uses a custom function for random filename generation.

	@showrefby
*/

int	heredoc_to_fd(t_word *content, enum e_status *status, t_globals *g)
{
	int		fd;
	char	*document;

	document = NULL;
	*status = heredoc_expand(content, &document, g);
	if (*status == E_MALLOC)
		return (-1);
	if (!document || !*document)
	{
		*status = SUCCESS;
		free(document);
		fd = xopen("/dev/null", O_RDONLY, NO_MODE);
		if (fd < 0)
			*status = E_OPEN;
		return (fd);
	}
	fd = make_heredoc_fd(document, status);
	if (fd < 0 && *status == SUCCESS)
		*status = ERROR;
	free(document);
	return (fd);
}

// int	heredoc_to_fd(t_word *content, t_globals *g)
// {
// 	int				fd;
// 	enum e_status	status;
// 	char			*document;

// 	document = NULL;
// 	status = heredoc_expand(content, &document, g);
// 	if (status == E_MALLOC)
// 		return (REDIR_ERR_MALLOC);
// 	if (!document || !*document)
// 	{
// 		free(document);
// 		fd = xopen("/dev/null", O_RDONLY, NO_MODE);
// 		return (fd);
// 	}
// 	fd = make_heredoc_fd(document, &status);
// 	if (status == E_MALLOC)
// 		fd = REDIR_ERR_MALLOC;
// 	free(document);
// 	return (fd);
// }

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static int	make_heredoc_fd(char *document, enum e_status *status)
{
	char	*filename;
	int		saved_errno;
	int		fd_read_only;
	int		fd_read_and_write;

	fd_read_only = -1;
	fd_read_and_write = make_temp_fd("sh-thd.", &filename, status);
	if (fd_read_and_write < 0)
		return (-1);
	saved_errno = heredoc_write(fd_read_and_write, document);
	if (saved_errno == SUCCESS)
	{
		fd_read_only = xopen(filename, O_RDONLY, S_IRUSR);
		saved_errno = errno;
	}
	close(fd_read_and_write);
	if (unlink(filename) < 0 && fd_read_only >= 0)
	{
		saved_errno = errno;
		close(fd_read_only);
		fd_read_only = -1;
	}
	free(filename);
	errno = saved_errno;
	return (fd_read_only);
}

static int	heredoc_write(int fd, char *heredoc)
{
	int				error;
	ssize_t			bytes_written;
	const size_t	herelen = ft_strlen(heredoc);

	errno = 0;
	bytes_written = write(fd, heredoc, herelen);
	error = errno;
	if (bytes_written < 0 || (size_t)bytes_written != herelen)
	{
		if (!error)
			return (ENOSPC);
		return (error);
	}
	return (SUCCESS);
}

/* **************************************************************** */
/*	                 Static Functions Documentation	            	*/
/* **************************************************************** */

/**
	@fn static int	make_heredoc_fd(char *document, enum e_status *status)

	@brief Does the work of creating a file descriptor for heredoc_to_fd().

	@li The file descriptor will have read permission only.

	@param document The content of the here-document.
	@param status A pointer to an enum e_status representing the status
	of the operation.

	@return File descriptor of the temporary file if successful,
	-1 on error.

	@note The file is first created with read and write permissions, then
	reopened with read permission only to avoid race conditions.

	@warning This function takes a \ar status argument, however it is only
	set by make_temp_fd() for malloc(2) error handling, other errors should
	rely on \ar errno. This is a planning error. Oops...

	@todo Add comment about errno.

	@showrefby
*/

/**
	@fn static int	heredoc_write(int fd, char *heredoc)

	@brief Writes \ar heredoc to \ar fd.

	@param fd The file descriptor to write to.
	@param heredoc The heredoc string to be written.

	@return Execution status:
	@li \ar SUCCESS The entire heredocument was successfully written.
	@li \ar errno The number of bytes written was less than the length
	of the document.

	@note If the number of bytes written was less than the length of the
	document but \ar errno is 0, returns \ar ENOSPC.

	@showrefby
*/