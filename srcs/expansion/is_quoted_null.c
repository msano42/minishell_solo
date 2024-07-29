#include <stdbool.h>

#include "minishell.h"

bool	is_quoted_null(const char *str)
{
	if (!str)
		return (false);
	return (str[0] == CTLNUL && str[1] == '\0');
}
