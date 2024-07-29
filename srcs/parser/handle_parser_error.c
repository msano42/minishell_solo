#include <unistd.h>

#include "parser.h"

#include "general.h"

#define EX_BADUSAGE	2

#define SYNTAX_MAX_HEREDOC_ERR_MSG "minshell: maximum here-document count \
exceeded\n"
#define SYNTAX_UNEXPECTED_EOF_ERR_MSG "syntax error: unexpected end of file\n"
#define SYNTAX_UNEXPECTED_TOKEN_ERR_FORMAT "minishell: syntax error near unexpected \
token `%s'\n"

static void			set_exit_value_from_parser_error(t_parser *parser);
static void			print_syntax_error(t_parser *parser);
static const char	*operator_token_to_str(enum e_token token);

void	handle_parser_error(t_parser *parser)
{
	/* move EX_BADUSAGE */
	if (parser->status == SUCCESS)
	{
		if (parser->subshell_stack || \
		(parser->command && !*(parser->current_node_addr)))
			parser->status = E_SYNTAX;
	}
	if (parser->status == E_SYNTAX || parser->status == E_MAX_HEREDOC)
		print_syntax_error(parser);
	set_exit_value_from_parser_error(parser);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static void	print_syntax_error(t_parser *parser)
{
	const char	*token_str;

	if (parser->status == E_MAX_HEREDOC)
	{
		ft_dprintf(STDERR_FILENO, SYNTAX_MAX_HEREDOC_ERR_MSG);
		return ;
	}
	if (parser->current_token == EOF)
	{
		ft_dprintf(STDERR_FILENO, SYNTAX_UNEXPECTED_EOF_ERR_MSG);
		return ;
	}
	if (parser->current_token == TOK_WORD)
		token_str = parser->element.word->word;
	else
		token_str = operator_token_to_str(parser->current_token);
	ft_dprintf(STDERR_FILENO, SYNTAX_UNEXPECTED_TOKEN_ERR_FORMAT, token_str);
}

static void	set_exit_value_from_parser_error(t_parser *parser)
{
	int	exit_value;

	exit_value = parser->globals->last_command_exit_value;
	if (parser->status == E_MALLOC)
		exit_value = 2;
	else if (parser->status == E_UNCLOSED_QUOTE)
	{
		if (!exit_value)
			exit_value = EX_BADUSAGE;
	}
	else if (parser->status == E_SYNTAX || parser->status == E_MAX_HEREDOC)
		exit_value = EX_BADUSAGE;
	parser->globals->last_command_exit_value = exit_value;
}

static const char	*operator_token_to_str(enum e_token token)
{
	if (token == TOK_EOF)
		return ("EOF");
	if (token == TOK_NEWLINE)
		return ("newline");
	if (token == TOK_L_PAREN)
		return ("(");
	if (token == TOK_R_PAREN)
		return (")");
	if (token == TOK_SEMICOLON)
		return (";");
	if (token == TOK_LESS)
		return ("<");
	if (token == TOK_GREATER)
		return (">");
	if (token == TOK_PIPE)
		return ("|");
	if (token == TOK_LESS_LESS)
		return ("<<");
	if (token == TOK_GREATER_GREATER)
		return (">>");
	if (token == TOK_AND_AND)
		return ("&&");
	if (token == TOK_OR_OR)
		return ("||");
	return (NULL);
}
