#include <stdio.h>
#include <stdbool.h>

#include "parser.h"
#include "general.h"
#include "tokenize_internal.h"

#define DOUBLE_TOKEN_CHARS "|&<>"

static bool	could_be_double_token(int c);
static int	classify_token(t_input *input);

enum e_token	get_token(t_parser *parser)
{
	int	token_type;

	if (parser->lookahead_token != TOK_UNDEFINED)
	{
		token_type = parser->lookahead_token;
		parser->lookahead_token = TOK_UNDEFINED;
		return (token_type);
	}
	token_type = classify_token(parser->input);
	parser->status = parser->input->status;
	parser->input->status = SUCCESS;
	if (token_type == TOK_WORD)
	{
		parser->element.word = parser->input->word;
		parser->input->word = NULL;
	}
	return (token_type);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

int	classify_token(t_input *input)
{
	int	c;
	int	next_char;

	c = shell_getc(REMOVE_BACKSLASH_NEWLINE, input);
	while (c != EOF && is_blank_char(c) && input->status == SUCCESS)
		c = shell_getc(REMOVE_BACKSLASH_NEWLINE, input);
	if (c == EOF && input->status == SUCCESS)
		input->eof_reached = true;
	if (c == EOF || c == '\n' || input->status != SUCCESS)
		return (c);
	if (is_meta_char(c))
	{
		if (could_be_double_token(c))
		{
			next_char = shell_getc(REMOVE_BACKSLASH_NEWLINE, input);
			if (next_char == c)
				return (c + DOUBLE_TOKEN_OFFSET);
			shell_ungetc(next_char, input);
		}
		if (c != '&' || input->status != SUCCESS)
			return (c);
	}
	get_token_word(c, input);
	return (TOK_WORD);
}

static bool	could_be_double_token(int c)
{
	return (!!ft_strchr(DOUBLE_TOKEN_CHARS, c));
}
