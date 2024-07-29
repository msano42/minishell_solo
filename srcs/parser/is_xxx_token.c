#include "parser.h"

bool	is_redirection_token(enum e_token token)
{
	return (token == TOK_LESS || \
			token == TOK_GREATER || \
			token == TOK_LESS_LESS || \
			token == TOK_GREATER_GREATER);
}

bool	is_connection_token(enum e_token token)
{
	return (token == TOK_SEMICOLON || \
			token == TOK_PIPE || \
			token == TOK_AND_AND || \
			token == TOK_OR_OR);
}

bool	is_end_of_command_token(enum e_token token)
{
	return (token == TOK_EOF || token == TOK_NEWLINE);
}
