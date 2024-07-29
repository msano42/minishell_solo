#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "command.h"
#include "general.h"
#include "builtins.h"
#include "builtins_internal.h"

static bool				n_option_is_set(t_word_list **list);

int	echo_builtin(t_word_list *list, t_globals *g)
{
	const bool	print_newline = !n_option_is_set(&list);

	(void)g;
	while (list)
	{
		if (list->word && list->word->word)
		{
			if (builtin_putstr(list->word->word, "echo") == E_WRITE)
				return (EXECUTION_FAILURE);
		}
		if (list->next)
		{
			if (builtin_putstr(" ", "echo") == E_WRITE)
				return (EXECUTION_FAILURE);
		}
		list = list->next;
	}
	if (print_newline)
	{
		if (builtin_putstr("\n", "echo") == E_WRITE)
			return (EXECUTION_FAILURE);
	}
	return (EXECUTION_SUCCESS);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

/* TODO: Convert to general getopt() or x_option_is_set(). */
static bool	n_option_is_set(t_word_list **list)
{
	size_t	i;
	char	*arg;
	bool	n_option;

	if (!list)
		return (false);
	n_option = false;
	while (*list)
	{
		if (!(*list)->word || !(*list)->word->word)
			break ;
		arg = (*list)->word->word;
		if (arg[0] != '-' || arg[1] == '\0')
			break ;
		i = 1;
		while (arg[i] && arg[i] == 'n')
			i++;
		if (arg[i])
			break ;
		n_option = true;
		(*list) = (*list)->next;
	}
	return (n_option);
}

/* **************************************************************** */
/*	                 Static Functions Documentation	            	*/
/* **************************************************************** */

/**
	@fn static bool	n_option_is_set(t_word_list **list)

	@brief  Checks if the \ar -n option is set in \ar *list.

	Updates \ar *list to the first word that isn't a valid \ar -n option
	string.

	@param list A pointer to a pointer to the beginning of the arguments.

	@return \ar true if the \ar -n option is set, \ar false otherwise.

	Examples of valid \ar -n option strings:
	@li "-n"
	@li "-nnn"

	Examples of invalid \ar -n option strings:
	@li "-"
	@li "n"
	@li "-x"
	@li "-nx"
	@li "-nxn"

	@showrefby
*/
/**
	@fn static enum e_status	echo_str(const char *str)
	@brief Outputs \ar str to the standard output, and prints an
	error message if the write(2) operation fails.

	@param str The string to output.

	@return Execution status:
	@li \ar SUCCESS
	@li \ar E_WRITE - write error

	@showrefby
*/