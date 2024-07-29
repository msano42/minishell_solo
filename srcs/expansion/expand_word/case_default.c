#include "expansion.h"
#include "variables.h"

void	case_default(const char *str, uint8_t word_flags, t_expansion *exp, \
		t_globals *g)
{
	const char	c = str[exp->idx];
	const char	buf[3] = {CTLESC, c, '\0'};

	if (c == ' ' && (g->ifs_is_null || word_flags & W_NOSPLIT))
		add_string_to_expansion(buf, exp);
	else if (in_dquotes_or_heredoc(exp) \
			|| (is_ifs(c, g) && !(word_flags & W_NOSPLIT)))
	{
		if (!in_dquotes_or_heredoc(exp))
			exp->has_quoted_ifs = true;
		add_string_to_expansion(buf, exp);
	}
	else
		add_string_to_expansion(&buf[1], exp);
	if (str[exp->idx])
		exp->idx++;
}

// else if ((c == '=' || c == ':')
// && !(is_ifs(c, g) && !(word_flags & W_NOSPLIT)))
// 	add_string_to_expansion(&buf[1], exp);
// else if (c == '~'
// && (!(is_ifs(c, g) && !(word_flags & W_NOSPLIT))
	// || in_dquotes_or_heredoc(exp)))
// 	add_string_to_expansion(&buf[1], exp);