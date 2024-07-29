#include "expansion.h"
#include "minishell.h"

#include <stddef.h>

void	remove_quoted_ifs_in_place(char *str, t_globals *g)
{
	char	c;
	size_t	i;
	size_t	j;

	if (!str)
		return ;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == CTLESC)
		{
			c = str[i + 1];
			if (c != '\0' && is_ifs(c, g))
				i++;
		}
		str[j++] = str[i++];
	}
	str[j] = '\0';
}
