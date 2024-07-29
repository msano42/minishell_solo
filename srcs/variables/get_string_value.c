#include <stddef.h>

#include "variables.h"

/*
	Returns the pointer to the value of a variable with name NAME, or NULL
	if the variable doesn't exist. (No allocation.)

	Unlike Bash, returns var->value directly instead of get_variable_value()
	which handles array variables.
*/
char	*get_string_value(const char *name, t_globals *g)
{
	t_shell_var	*var;

	var = find_variable(name, g);
	if (var)
		return (var->value);
	return (NULL);
}

/* Return the string value of a variable.  Return NULL if the variable
   doesn't exist.  Don't cons a new string.  This is a potential memory
   leak if the variable is found in the temporary environment, but doesn't
   leak in practice.  Since functions and variables have separate name
   spaces, returns NULL if var_name is a shell function only. */