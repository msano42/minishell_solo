#include "minishell.h"
#include "wrappers.h" // memory
#include "command.h"
#include "redirection.h"

void	destroy_redirects(t_redirect *list)
{
	t_redirect	*current;

	while (list)
	{
		current = list;
		free(current->here_doc_eof);
		if (current->instruction == R_OUTPUT_DIRECTION || \
			current->instruction == R_INPUT_DIRECTION || \
			current->instruction == R_APPENDING_TO || \
			current->instruction == R_READING_UNTIL)
			destroy_word(current->redirectee.filename);
		list = list->next;
		free(current);
	}
}
