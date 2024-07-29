#include "parser.h"

void	push_here_document(t_parser *parser)
{
	if (parser->heredoc_count >= HEREDOC_MAX)
	{
		parser->status = E_MAX_HEREDOC;
		destroy_redirects(parser->element.redirect);
		parser->element.redirect = NULL;
		return ;
	}
	parser->heredoc_stack[parser->heredoc_count++] = parser->element.redirect;
	return ;
}
