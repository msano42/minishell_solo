#include "expansion.h"

#include <stdbool.h>

bool	in_dquotes_or_heredoc(t_expansion *vars)
{
	if (!vars)
		return (false);
	return (vars->in_dquotes || vars->in_heredoc);
}
