#include <stdbool.h>

#include "command.h"

static bool	is_stdin_redirection(enum e_redir_type type, int redirector);

/**
	@fn bool	has_stdin_redirects(t_redirect *list)

	@brief Checks if a linked list of redirections contains any stdin
	redirections.

	@param list The list of redirections to check.

	@return \ar true if the list contains at least one stdin redirection,
	\ar false otherwise.

	@seebash @verbatim stdin_redirects() @endverbatim

	@showrefby
*/
bool	has_stdin_redirects(t_redirect *list)
{
	while (list)
	{
		if (is_stdin_redirection(list->instruction, list->redirector.fd))
			return (true);
		list = list->next;
	}
	return (false);
}

#if 0

int	count_stdin_redirects(t_redirect *list)
{
	int	cnt;

	cnt = 0;
	while (list)
	{
		if (is_stdin_redirection(list->instruction, list->redirector.dest))
			cnt++;
		list = list->next;
	}
	return (cnt);
}
#endif

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static bool	is_stdin_redirection(enum e_redir_type type, int redirector)
{
	if (type == R_INPUT_DIRECTION || \
		type == R_READING_UNTIL)
		return (true);
	if (type == R_DUPLICATING_INPUT || \
		type == R_CLOSE_THIS)
		return (redirector == 0);
	if (type == R_OUTPUT_DIRECTION || \
		type == R_APPENDING_TO || \
		type == R_DUPLICATING_OUTPUT)
		return (false);
	return (false);
}
