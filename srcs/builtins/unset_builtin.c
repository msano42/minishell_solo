#include <unistd.h>

#include "general.h"
#include "variables.h"
#include "builtins.h"
#include "builtins_internal.h"

#define UNSET_ERR_FORMAT "minishell: unset: `%s': not a valid identifier\n"

#ifndef STRICT_BASH_52

int	unset_builtin(t_word_list *list, t_globals *g)
{
	int			status;
	char		*name;

	if (check_options(&list, "unset", UNSET_USAGE, UNSET_LONGDOC) == EX_BADUSAGE)
		return (EX_BADUSAGE);
	status = EXECUTION_SUCCESS;
	while (list)
	{
		if (list->word)
		{
			name = list->word->word;
			if (name && is_legal_identifier(name))
			{
				unbind_variable(name, g);
				handle_if_special_variable_modified(name, g);
			}
			else
			{
				ft_dprintf(STDERR_FILENO, INVALID_ID_ERR_FORMAT, "unset", name);
				status = EXECUTION_FAILURE;
			}
		}
		list = list->next;
	}
	return (status);
}

#else

int	unset_builtin(t_word_list *list, t_globals *g)
{
	int			status;
	char		*name;

	if (check_opptions(list, "unset", UNSET_USAGE, UNSET_LONGDOC) == EX_BADUSAGE)
		return (EX_BADUSAGE);
	status = EXECUTION_SUCCESS;
	while (list)
	{
		if (list->word && list->word->word)
		{
			name = list->word->word;
			unbind_variable(name, g);
			handle_if_special_variable_modified(name, g);
		}
		list = list->next;
	}
	return (status);
}

#endif /* !STRICT_BASH_52 */
