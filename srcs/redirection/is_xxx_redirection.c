#include "command.h"

#include <stdbool.h>

bool	is_io_redirection(enum e_redir_type type)
{
	return (type == R_OUTPUT_DIRECTION || \
			type == R_APPENDING_TO || \
			type == R_INPUT_DIRECTION);
}

bool	is_dup_redirection(enum e_redir_type type)
{
	return (type == R_DUPLICATING_INPUT || \
			type == R_DUPLICATING_OUTPUT);
}

/**
	@fn bool	is_expandable_filename_redirection(t_redirect *redirect)

	@brief Checks if a redirection instruction involves an expandable filename.

	@param redirect Pointer to the redirection to check.

	@return \ar true if the redirection instruction involves an expandable
	filename, \ar false otherwise.

	@seebash @verbatim expandable_redirection_filename() @endverbatim

	@showrefby
*/
bool	is_expandable_filename_redirection(t_redirect *redirect)
{
	if (!redirect)
		return (false);
	return (redirect->instruction == R_OUTPUT_DIRECTION || \
			redirect->instruction == R_APPENDING_TO || \
			redirect->instruction == R_INPUT_DIRECTION);
}
