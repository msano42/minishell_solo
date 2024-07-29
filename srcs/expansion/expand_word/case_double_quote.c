#include "expansion.h"
#include "general.h"
#include "wrappers.h" // memory

static char	*get_double_quote_expansion(const char *str, t_expansion *exp, \
			t_globals *g);
static char	*call_expand_word_internal_recursive(const char *str, \
			t_expansion *exp, t_globals *g);
static char	*extract_dquoted_expansion_from_list(t_word_list *list);

/*
	This function will only recurse once per invocation,
	as double quotes within double quotes are not treated specially.
*/
void	case_double_quote(const char *str, uint8_t word_flags, \
		t_expansion *exp, t_globals *g)
{
	char	*expansion;

	if (in_dquotes_or_heredoc(exp))
	{
		add_char_to_expansion(str[exp->idx++], exp);
		return ;
	}
	expansion = get_double_quote_expansion(str, exp, g);
	if (exp->status == E_MALLOC)
		return ;
	if (!expansion && exp->quoted_state == PARTIALLY_QUOTED)
	{
		exp->had_quoted_null = true;
		if (word_flags & W_NOSPLIT)
			return ;
	}
	add_quoted_string_to_expansion(expansion, exp);
	free(expansion);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static char	*get_double_quote_expansion(const char *str, t_expansion *exp, \
			t_globals *g)
{
	size_t	start;
	char	*expansion;
	char	*dquoted_section;

	start = exp->idx++;
	dquoted_section = extract_double_quoted_section(str, &(exp->idx));
	if (!dquoted_section)
	{
		exp->status = E_MALLOC;
		return (NULL);
	}
	if (start == 0 && str[exp->idx] == '\0')
		exp->quoted_state = WHOLLY_QUOTED;
	else
		exp->quoted_state = PARTIALLY_QUOTED;
	if (*dquoted_section == '\0')
	{
		exp->had_quoted_null = true;
		free(dquoted_section);
		return (NULL);
	}
	expansion = call_expand_word_internal_recursive(dquoted_section, exp, g);
	free(dquoted_section);
	return (expansion);
}

static char	*call_expand_word_internal_recursive(const char *str, \
			t_expansion *exp, t_globals *g)
{
	t_word_list	*list;
	t_word		temp_word;
	t_expansion	temp_exp;

	temp_word.word = (char *)str;
	temp_word.flags = 0;
	init_expansion_struct(&temp_exp, Q_DOUBLE_QUOTES, true);
	list = expand_word(&temp_word, &temp_exp, g);
	if (temp_exp.status == E_MALLOC)
	{
		exp->status = E_MALLOC;
		return (NULL);
	}
	if (!list)
		return (NULL);
	if (list->word && (list->word->flags & W_HASQUOTEDNULL))
		exp->had_quoted_null = true;
	return (extract_dquoted_expansion_from_list(list));
}

static char	*extract_dquoted_expansion_from_list(t_word_list *list)
{
	char	*str;
	uint8_t	flags;

	dequote_list(list);
	flags = list->word->flags;
	str = list->word->word;
	list->word->word = NULL;
	destroy_word_list(list);
	if ((flags & W_HASQUOTEDNULL) && is_quoted_null(str))
		remove_quoted_nulls_in_place(str);
	return (str);
}
