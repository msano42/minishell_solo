#include <stddef.h>
#include <stdbool.h>

#include "general.h"

// #define CBSDQUOTE "\n\"$\\" // removed backquote

static bool	include_backslash(bool in_dquotes, int c);

void	quote_removal_in_place(char *str)
{
	size_t	i;
	size_t	j;
	bool	in_dquotes;

	i = 0;
	j = 0;
	in_dquotes = false;
	while (1)
	{
		if (str[i] == '"')
			in_dquotes = !in_dquotes;
		else if (str[i] == '\'' && !in_dquotes)
		{
			while (str[++i] && str[i] != '\'')
				str[j++] = str[i];
			if (!str[i])
				str[j] = str[i];
		}
		else
		{
			if (str[i] == '\\' && include_backslash(in_dquotes, str[++i]))
				str[j++] = '\\';
			str[j++] = str[i];
		}
		if (!str[i++])
			break ;
	}
}

static bool	include_backslash(bool in_dquotes, int c)
{
	if (c == '\0')
		return (true);
	return (in_dquotes && !ft_strchr("\n\"$\\", c));
}
