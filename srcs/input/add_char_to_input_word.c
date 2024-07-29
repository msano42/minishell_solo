#include "minishell.h"
#include "parser.h"
#include "general.h"
#include "wrappers.h" // memory

/* TODO: Implement buffer management because this is inefficient as fuck. */
enum e_status	add_char_to_input_word(char c, bool flag_dollar, \
				bool quote_escape, t_input *input)
{
	char		*old_word;
	const char	buf[3] = {CTLESC, c, '\0'};

	if (flag_dollar && c == '$')
		input->word->flags |= W_HASDOLLAR;
	old_word = input->word->word;
	if (quote_escape && (c == CTLESC || c == CTLNUL))
		input->word->word = ft_strjoin(old_word, buf);
	else
		input->word->word = ft_strjoin(old_word, &buf[1]);
	if (!input->word->word)
		input->status = E_MALLOC;
	free(old_word);
	return (input->status);
}
