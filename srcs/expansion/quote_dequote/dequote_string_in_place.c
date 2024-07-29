#include <stddef.h>
#include "expansion.h"

void	dequote_string_in_place(char *str)
{
	size_t	i;
	size_t	j;

	if (!str)
		return ;
	if (str[0] == CTLESC && str[1] == '\0')
		return ;
	if (str[0] == CTLNUL && str[1] == '\0')
	{
		str[0] = '\0';
		return ;
	}
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == CTLESC)
		{
			i++;
			if (!str[i])
				break ;
		}
		str[j++] = str[i++];
	}
	str[j] = '\0';
}
