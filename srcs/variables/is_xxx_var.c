#include "variables.h"

bool	is_imported_var(t_shell_var *var)
{
	if (!var)
		return (false);
	return ((var->attributes & ATT_IMPORTED) != 0);
}

bool	is_exported_var(t_shell_var *var)
{
	if (!var)
		return (false);
	return ((var->attributes & ATT_EXPORTED) != 0);
}

bool	is_invisible_var(t_shell_var *var)
{
	if (!var)
		return (false);
	return ((var->attributes & ATT_INVISIBLE) != 0);
}
