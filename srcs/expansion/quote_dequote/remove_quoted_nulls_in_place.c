#include "minishell.h"

#include <stddef.h>

void	remove_quoted_nulls_in_place(char *str)
{
	size_t	i;
	size_t	j;

	if (!str)
		return ;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == CTLNUL)
			i++;
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
}
