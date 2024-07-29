#include <stdio.h>

#include "minishell.h"
#include "general.h"
#include "parser.h"
#include "wrappers.h" // memory
#include "tokenize_internal.h"

#define PASS_NEXT 1
#define DONT_PASS_NEXT 0

#define END_OF_WORD_CHARS "\t\n &();<>|"

static void	read_word(int c, t_input *input);
static bool	is_end_of_word(int c, t_input *input);
static bool	handle_backslash_in_word(t_input *input);

void	get_token_word(int c, t_input *input)
{
	input->word = alloc_word();
	if (!input->word)
	{
		input->status = E_MALLOC;
		return ;
	}
	read_word(c, input);
	if (input->status != SUCCESS)
	{
		destroy_word(input->word);
		input->word = NULL;
	}
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static void	read_word(int c, t_input *input)
{
	bool	pass_next_character;

	pass_next_character = false;
	while (c != EOF && input->status == SUCCESS)
	{
		if (pass_next_character)
		{
			add_char_to_input_word(c, FLAG_DOLLAR, IGN_ESC, input);
			pass_next_character = false;
		}
		else if (c == '\\')
			pass_next_character = handle_backslash_in_word(input);
		else if (is_quote_char(c))
			add_quoted_section(c, input);
		else if (is_end_of_word(c, input))
			break ;
		else
			add_char_to_input_word(c, FLAG_DOLLAR, QUOTE_ESC, input);
		if (input->status != SUCCESS)
			break ;
		c = shell_getc(!pass_next_character, input);
	}
}

static bool	is_end_of_word(int c, t_input *input)
{
	char	next_char;

	if (c == '&')
	{
		next_char = shell_getc(REMOVE_BACKSLASH_NEWLINE, input);
		shell_ungetc('&', input);
		if (next_char == '&')
		{
			shell_ungetc('&', input);
			return (true);
		}
		return (false);
	}
	if (!ft_strchr(END_OF_WORD_CHARS, c))
		return (false);
	shell_ungetc(c, input);
	return (true);
}

static bool	handle_backslash_in_word(t_input *input)
{
	int	next_char;

	next_char = shell_getc(KEEP_BACKSLASH_NEWLINE, input);
	if (input->status != SUCCESS)
		return (DONT_PASS_NEXT);
	if (next_char == '\n')
	{
		input->rl_current_prompt = input->rl_secondary_prompt;
		return (DONT_PASS_NEXT);
	}
	else
	{
		input->word->flags |= W_QUOTED;
		shell_ungetc(next_char, input);
		add_char_to_input_word('\\', IGN_DOLLAR, IGN_ESC, input);
		return (PASS_NEXT);
	}
}
