#include <stdint.h>
#include <unistd.h>

#include "variables.h"
#include "general.h"
#include "builtins_internal.h"

static size_t	assignment_index(const char *str);
static uint8_t	split_assign_in_place(char *str, \
				size_t assignment_index);
static void		restore_assignment_str_in_place(char *str, \
				size_t assignment_idx, int aflags);

int	export_variable(char *name, t_globals *g)
{
	t_shell_var		*var;
	char			*value;
	const size_t	assignment_idx = assignment_index(name);
	const uint8_t	bind_flags = split_assign_in_place(name, assignment_idx);

	value = NULL;
	if (assignment_idx)
		value = &name[assignment_idx + 1];
	if (!is_legal_identifier(name))
	{
		ft_dprintf(STDERR_FILENO, INVALID_ID_ERR_FORMAT, "export", name);
		return (EXECUTION_FAILURE);
	}
	var = find_variable(name, g);
	if (assignment_idx || !var)
	{
		var = bind_variable(name, value, bind_flags, g);
		if (!var)
			return (EXECUTION_MALLOC_FAILURE);
		var->attributes |= (ATT_INVISIBLE * !assignment_idx);
		handle_if_special_variable_modified(name, g);
	}
	var->attributes |= ATT_EXPORTED;
	restore_assignment_str_in_place(name, assignment_idx, bind_flags);
	return (EXECUTION_SUCCESS);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

/*
	Returns the index of '=' or 0 if the string isn't a valid
	assignment string. Handles "+=".
*/
static size_t	assignment_index(const char *str)
{
	size_t	i;

	if (!str || !*str)
		return (0);
	if (!is_legal_variable_starter(*str))
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (i);
		if (str[i] == '+' && str[i + 1] == '=')
			return (i + 1);
		if (!is_legal_variable_char(str[i]))
			return (0);
		i++;
	}
	return (0);
}

/* NB: Returns flags to pass to bind_variable() */
static uint8_t	split_assign_in_place(char *str, size_t assignment_index)
{
	if (!assignment_index)
		return (0);
	str[assignment_index] = '\0';
	if (str[assignment_index - 1] == '+')
	{
		str[assignment_index - 1] = '\0';
		return (ASS_APPEND);
	}
	return (0);
}

static void	restore_assignment_str_in_place(char *str, size_t assignment_idx, \
			int aflags)
{
	if (!assignment_idx)
		return ;
	str[assignment_idx] = '=';
	if (aflags & ASS_APPEND)
		str[assignment_idx - 1] = '+';
}
