#include "variables.h"

void	free_variable_hash_data(void *data)
{
	t_shell_var	*var;

	if (!data)
		return ;
	var = (t_shell_var *)data;
	destroy_variable(var);
}
