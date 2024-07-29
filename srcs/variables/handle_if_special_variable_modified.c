#include "general.h"
#include "hashlib.h"
#include "variables.h"

#include <string.h> // ft_strncmp()

void	handle_if_special_variable_modified(const char *name, t_globals *g)
{
	if (!name || !g)
		return ;
	if (!ft_strncmp("IFS", name, 4))
		set_ifs(find_variable("IFS", g), g);
}

// void	handle_if_special_variable_modified(const char *name, t_globals *g)
// {
// 	if (!name || !g)
// 		return ;
// 	if (!ft_strncmp("PATH", name, 5))  /* for testing */
// 		hash_flush(g->hashed_filenames, free_variable_hash_data);
// 		// hash_flush(g->hashed_filenames, phash_freedata);
// 	if (!ft_strncmp("IFS", name, 4))
// 		set_ifs(find_variable("IFS", g), g);
// }
