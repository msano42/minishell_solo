#include <fcntl.h>
#include <stddef.h>

#include "command.h"
#include "wrappers.h" // memory

t_redirect	*make_redirection(t_redir_target source, enum e_redir_type type, \
			t_redir_target dest)
{
	t_redirect	*redirect;

	redirect = xmalloc(sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	redirect->instruction = type;
	redirect->redirector = source;
	redirect->redirectee = dest;
	redirect->open_flags = 0;
	redirect->here_doc_eof = NULL;
	redirect->next = NULL;
	if (type == R_OUTPUT_DIRECTION)
		redirect->open_flags = (O_TRUNC | O_WRONLY | O_CREAT);
	else if (type == R_APPENDING_TO)
		redirect->open_flags = (O_APPEND | O_WRONLY | O_CREAT);
	else if (type == R_INPUT_DIRECTION)
		redirect->open_flags = O_RDONLY;
	return (redirect);
}
