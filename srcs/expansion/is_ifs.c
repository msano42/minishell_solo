#include <stdbool.h>

#include "variables.h"

bool	is_ifs(char c, t_globals *g)
{
	if (!g)
		return (false);
	return (g->ifs_cmap[(unsigned char)c] != 0);
}
