#include "expansion.h"
#include "wrappers.h" // memory

t_word_list	*expand_word(t_word *word, t_expansion *exp, t_globals *g)
{
	while (word->word && word->word[exp->idx] && exp->status != E_MALLOC)
	{
		if (word->word[exp->idx] == CTLESC)
			case_ctlesc(word->word, exp);
		else if (word->word[exp->idx] == '$')
		{
			exp->expanded_something = true;
			case_dollar(word->word, exp, g);
		}
		else if (word->word[exp->idx] == '\\')
			case_backslash(word->word, exp, g);
		else if (word->word[exp->idx] == '"')
			case_double_quote(word->word, word->flags, exp, g);
		else if (word->word[exp->idx] == '\'')
			case_single_quote(word->word, word->flags, exp, g->ifs_value);
		else
			case_default(word->word, word->flags, exp, g);
	}
	if (exp->status == E_MALLOC)
	{
		free(exp->expanded_str);
		exp->expanded_str = NULL;
		return (NULL);
	}
	return (finish_expansion(word->flags, exp, g));
}
