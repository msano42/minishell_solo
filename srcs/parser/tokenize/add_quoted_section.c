#include <unistd.h>

#include "minishell.h"
#include "general.h"
#include "parser.h"
#include "tokenize_internal.h"

#define UNCLOSED_PAIR_ERR_FORMAT "minishell: unexpected EOF while looking \
for matching `%c'\n"

static void	add_quoted_section_internal(int c, int quote, t_input *input);
static void	handle_pass_next_in_quotes(int *c, bool *pass_next, int quote, \
			t_input *input);

void	add_quoted_section(int quote, t_input *input)
{
	const char	*old_prompt = input->rl_current_prompt;

	input->quote = quote;
	input->word->flags |= W_QUOTED;
	input->rl_current_prompt = input->rl_secondary_prompt;
	add_quoted_section_internal(quote, quote, input);
	if (input->status == SUCCESS)
		add_char_to_input_word(quote, IGN_DOLLAR, IGN_ESC, input);
	input->rl_current_prompt = old_prompt;
	input->quote = 0;
}

static void	add_quoted_section_internal(int c, int quote, t_input *input)
{
	bool	pass_next_character;

	pass_next_character = true;
	while (input->status == SUCCESS)
	{
		if (c == EOF)
		{
			input->eof_reached = true;
			ft_dprintf(STDERR_FILENO, UNCLOSED_PAIR_ERR_FORMAT, quote);
			input->status = E_UNCLOSED_QUOTE;
			return ;
		}
		if (pass_next_character)
		{
			handle_pass_next_in_quotes(&c, &pass_next_character, quote, input);
			continue ;
		}
		else if (c == '\\')
			pass_next_character = true;
		else if (c == quote)
			break ;
		if (add_char_to_input_word(c, quote == '"', QUOTE_ESC, input))
			return ;
		c = shell_getc((quote != '\'' && !pass_next_character), input);
	}
}

static void	handle_pass_next_in_quotes(int *c, bool *pass_next, int quote, \
			t_input *input)
{
	*pass_next = false;
	if (*c == CTLESC)
		add_char_to_input_word(*c, IGN_DOLLAR, QUOTE_ESC, input);
	else
		add_char_to_input_word(*c, IGN_DOLLAR, IGN_ESC, input);
	if (input->status != SUCCESS)
		return ;
	*c = shell_getc(quote != '\'', input);
}
