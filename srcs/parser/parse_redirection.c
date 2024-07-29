#include <unistd.h>

#include "minishell.h"
#include "command.h"
#include "parser.h"
#include "parser_internal.h"

static t_redirect			*call_make_redirection(t_parser *parser);
static enum e_redir_type	redir_token_to_redir_type(enum e_token token);

void	parse_redirection(t_parser *parser)
{
	enum e_token	next_token;

	next_token = get_token(parser);
	if (parser->status != SUCCESS)
		return ;
	if (next_token != TOK_WORD)
	{
		parser->status = E_SYNTAX;
		parser->current_token = next_token;
		return ;
	}
	parser->element.redirect = call_make_redirection(parser);
	if (!parser->element.redirect)
	{
		parser->status = E_MALLOC;
		destroy_word(parser->element.word);
		parser->element.word = NULL;
		return ;
	}
	if (parser->current_token == TOK_LESS_LESS)
		push_here_document(parser);
	parser->element.word = NULL;
	parser->current_token = TOK_WORD;
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static t_redirect	*call_make_redirection(t_parser *parser)
{
	t_redir_target		dest;
	t_redir_target		source;
	enum e_redir_type	redir_type;

	redir_type = redir_token_to_redir_type(parser->current_token);
	if (redir_type == R_INPUT_DIRECTION || redir_type == R_READING_UNTIL)
		source.fd = STDIN_FILENO;
	else
		source.fd = STDOUT_FILENO;
	dest.filename = parser->element.word;
	return (make_redirection(source, redir_type, dest));
}

/* Consider changing default value. */
/* TODO: Make e_redir_type and e_token have same values. */
static enum e_redir_type	redir_token_to_redir_type(enum e_token token)
{
	if (token == TOK_LESS)
		return (R_INPUT_DIRECTION);
	if (token == TOK_GREATER)
		return (R_OUTPUT_DIRECTION);
	if (token == TOK_LESS_LESS)
		return (R_READING_UNTIL);
	if (token == TOK_GREATER_GREATER)
		return (R_APPENDING_TO);
	return (0);
}
