#include "expansion.h"

#include <stdbool.h>

void	init_expansion_struct(t_expansion *exp, uint8_t quoted, \
		bool ign_expanded_something)
{
	exp->status = SUCCESS;
	exp->in_heredoc = false;
	exp->in_dquotes = false;
	if (quoted & Q_HERE_DOCUMENT)
		exp->in_heredoc = true;
	if (quoted & Q_DOUBLE_QUOTES)
		exp->in_dquotes = true;
	exp->idx = 0;
	exp->expanded_str = NULL;
	exp->quoted_state = UNQUOTED;
	exp->had_quoted_null = false;
	exp->has_quoted_ifs = false;
	exp->expanded_something = false;
	exp->ign_expanded_something = ign_expanded_something;
}
