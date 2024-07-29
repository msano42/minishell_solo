#include "minishell.h"

#include <stddef.h>
#include <stdbool.h>

void	dequote_escapes_in_place(char *str, const char *ifs_value)
{
	char		c;
	size_t		i;
	size_t		j;
	const bool	quote_spaces = (ifs_value && !*ifs_value);

	if (!str)
		return ;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == CTLESC)
		{
			c = str[i + 1];
			if (c == CTLESC || c == CTLNUL || (quote_spaces && c == ' '))
				i++;
		}
		str[j++] = str[i++];
	}
	str[j] = '\0';
}
