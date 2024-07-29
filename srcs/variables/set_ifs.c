#include <stddef.h>

#include "general.h"
#include "variables.h"

void	set_ifs(t_shell_var *ifs, t_globals *g)
{
	size_t			i;
	unsigned char	uc;

	g->ifs_var = ifs;
	g->ifs_value = " \t\n";
	if (ifs && ifs->value)
		g->ifs_value = ifs->value;
	g->ifs_is_set = (!!g->ifs_var);
	g->ifs_is_null = (g->ifs_is_set && !*(g->ifs_value));
	ft_memset(g->ifs_cmap, '\0', sizeof(g->ifs_cmap));
	i = 0;
	while ((g->ifs_value)[i])
	{
		uc = (g->ifs_value)[i];
		(g->ifs_cmap)[uc] = 1;
		i++;
	}
	g->ifs_firstc = '\0';
	if (g->ifs_value)
		g->ifs_firstc = *(g->ifs_value);
}
