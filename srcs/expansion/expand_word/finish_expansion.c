#include "expansion.h"
#include "command.h"
#include "general.h"
#include "wrappers.h" // memory

static uint8_t	get_final_expansion_flags(uint8_t original_flags, \
				t_expansion *exp);
static char		*get_final_expansion_string(uint8_t original_flags, \
				t_expansion *exp, t_globals *g);

t_word_list	*finish_expansion(uint8_t original_flags, t_expansion *exp, \
			t_globals *g)
{
	t_word		*word;
	t_word_list	*list;
	uint8_t		final_flags;
	char		*final_string;

	final_flags = get_final_expansion_flags(original_flags, exp);
	final_string = get_final_expansion_string(original_flags, exp, g);
	if (final_string != exp->expanded_str)
		free(exp->expanded_str);
	exp->expanded_str = NULL;
	if (exp->status == E_MALLOC || !final_string)
		return (NULL);
	word = alloc_word();
	list = make_word_list(word, NULL);
	if (!list || !word)
	{
		free(final_string);
		destroy_word(word);
		destroy_word_list(list);
		exp->status = E_MALLOC;
		return (NULL);
	}
	word->word = final_string;
	word->flags = final_flags;
	return (list);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static uint8_t	get_final_expansion_flags(uint8_t original_flags, \
				t_expansion *exp)
{
	uint8_t	final_flags;

	final_flags = 0;
	if (!exp->expanded_str || !exp->expanded_str[0])
	{
		if (exp->had_quoted_null || exp->quoted_state == PARTIALLY_QUOTED)
			final_flags |= W_HASQUOTEDNULL;
	}
	else
	{
		if (exp->had_quoted_null && is_quoted_null(exp->expanded_str))
			final_flags |= W_HASQUOTEDNULL;
		else if (exp->had_quoted_null && !(original_flags & W_NOSPLIT))
			final_flags |= W_SAWQUOTEDNULL;
		if (original_flags & W_NOGLOB)
			final_flags |= W_NOGLOB;
		if (exp->quoted_state == WHOLLY_QUOTED && !(original_flags & W_NOSPLIT))
			final_flags |= W_QUOTED;
	}
	if (in_dquotes_or_heredoc(exp))
		final_flags |= W_QUOTED;
	return (final_flags);
}

static char	*get_final_expansion_string(uint8_t original_flags, \
			t_expansion *exp, t_globals *g)
{
	char	*final_string;

	if (!exp->expanded_str || !exp->expanded_str[0])
	{
		if (exp->had_quoted_null || exp->quoted_state == PARTIALLY_QUOTED)
			final_string = ft_strdup(CTLNUL_STR);
		else
			return (NULL);
	}
	else if (original_flags & W_NOSPLIT)
		final_string = exp->expanded_str;
	else
	{
		if (!exp->ign_expanded_something && !exp->expanded_something \
			&& exp->has_quoted_ifs)
			remove_quoted_ifs_in_place(exp->expanded_str, g);
		final_string = exp->expanded_str;
	}
	if (!final_string)
		exp->status = E_MALLOC;
	return (final_string);
}
