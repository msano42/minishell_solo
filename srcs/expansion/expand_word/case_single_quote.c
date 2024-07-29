#include "expansion.h"
#include "wrappers.h" // memory
#include "command.h"

static char	*get_single_quote_expansion(const char *str, t_expansion *exp, \
			const char *ifs_value);

void	case_single_quote(const char *str, uint8_t word_flags, \
		t_expansion *exp, const char *ifs_value)
{
	char	*expansion;

	if (in_dquotes_or_heredoc(exp))
	{
		add_char_to_expansion(str[exp->idx++], exp);
		return ;
	}
	expansion = get_single_quote_expansion(str, exp, ifs_value);
	if (exp->status == E_MALLOC)
		return ;
	if (!expansion && exp->quoted_state == PARTIALLY_QUOTED)
	{
		exp->had_quoted_null = true;
		if (word_flags & W_NOSPLIT)
			return ;
	}
	if (!expansion)
	{
		add_char_to_expansion(CTLNUL, exp);
		return ;
	}
	add_quoted_string_to_expansion(expansion, exp);
	free(expansion);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static char	*get_single_quote_expansion(const char *str, t_expansion *exp, \
			const char *ifs_value)
{
	size_t	start;
	char	*expansion;

	start = exp->idx++;
	expansion = extract_single_quoted_section(str, &(exp->idx));
	if (!expansion)
	{
		exp->status = E_MALLOC;
		return (NULL);
	}
	if (start == 0 && str[exp->idx] == '\0')
		exp->quoted_state = WHOLLY_QUOTED;
	else
		exp->quoted_state = PARTIALLY_QUOTED;
	if (*expansion == '\0')
	{
		free(expansion);
		return (NULL);
	}
	dequote_escapes_in_place(expansion, ifs_value);
	return (expansion);
}
