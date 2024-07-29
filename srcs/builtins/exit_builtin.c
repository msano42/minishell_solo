#include <stdlib.h>
#include <unistd.h>

#include "general.h"
#include "command.h"
#include "builtins.h"
#include "builtins_internal.h"

#define EXIT_TOO_MANY_ERR_FORMAT "minishell: exit: too many arguments\n"
#define EXIT_NOTNUM_ERR_FORMAT "minishell: exit: %s: numeric argument \
required\n"

static int	get_exit_status(t_word_list *list, t_globals *g);

int	exit_builtin(t_word_list *list, t_globals *g)
{
	if (list && list->word && is_help_option(list->word->word))
	{
		print_usage("exit", EXIT_USAGE, EXIT_LONGDOC);
		g->last_command_exit_value = EX_BADUSAGE;
		return (EX_BADUSAGE);
	}
	g->really_exit = true; /* XXX */
	if (g->interactive)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (list)
		g->last_command_exit_value = get_exit_status(list, g);
	if (g->really_exit) /* XXX */
		exit(g->last_command_exit_value); /* XXX */
	return (g->last_command_exit_value); /* XXX */
	// jump_to_top_level(EXITBLTIN)
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static int	get_exit_status(t_word_list *list, t_globals *g)
{
	char		*arg;
	intmax_t	value;
	int			exit_status;

	if (list && list->word && is_end_of_options(list->word->word))
		list = list->next;
	if (!list || !list->word)
		return (g->last_command_exit_value);
	arg = list->word->word;
	if (!arg || !is_legal_number(arg, &value))
	{
		if (!arg)
			arg = "`'";
		ft_dprintf(STDERR_FILENO, EXIT_NOTNUM_ERR_FORMAT, arg);
		return (EX_BADUSAGE);
	}
	if (list->next)
	{
		g->really_exit = false; /* XXX */
		ft_dprintf(STDERR_FILENO, EXIT_TOO_MANY_ERR_FORMAT);
		return (EXECUTION_FAILURE); // jump_to_top_level(DISCARD)
	}
	exit_status = value & 255; // UINT8_MAX?
	return (exit_status);
}

/* **************************************************************** */
/*	                 Static Functions Documentation	            	*/
/* **************************************************************** */

/**
	@fn static int	get_exit_status(t_word_list *list, t_globals *g)

	@brief Processes \ar list and returns an exit status for the exit builtin.

	@li If no argument is given, returns \ar g->last_command_exit_value.

	@li If there are multiple arguments, prints an error message, sets
	\ar g->really_exit to \ar false and returns \ar EXECUTION_FAILURE.

	@li If the argument is not a legal number, prints an error message
	and returns \ar EX_BADUSAGE (=2).

	@note Bash-5.2 returns \ar EX_BADUSAGE (=2), but Bash-3.2 returns \ar 255.

	@param list	The arguments.
	@param g	Pointer to the global variables struct.

	@return Exit status.

	@showrefby
*/