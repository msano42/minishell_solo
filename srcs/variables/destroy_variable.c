#include "variables.h"
#include "wrappers.h" // memory

/*
	The calling function MUST set array_needs_making to 1 before
	calling this function if VAR has the ATT_EXPORTED attribute.
*/
void	destroy_variable(t_shell_var *var)
{
	if (!var)
		return ;
	free(var->value);
	free(var->exportstr);
	free(var->name);
	free(var);
}

	// if (var->attributes & ATT_EXPORTED)
	// 	array_needs_making = 1;