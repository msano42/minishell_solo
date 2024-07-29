#include <stdbool.h>
#include <unistd.h>

#include "general.h"
#include "builtins.h"
#include "builtins_internal.h"
#include "export_internal.h"

static int	export_variables_in_list(t_word_list *list, t_globals *g);

int	export_builtin(t_word_list *list, t_globals *g)
{
	int	status;

	if (check_options(&list, "export", EXPORT_USAGE, EXPORT_LONGDOC) \
		== EX_BADUSAGE)
		return (EX_BADUSAGE);
	if (list)
	{
		g->array_needs_making = true;
		status = export_variables_in_list(list, g);
	}
	else
		status = show_exported_variables(g);
	return (status);
}

static int	export_variables_in_list(t_word_list *list, t_globals *g)
{
	int		status;
	bool	any_failed;

	any_failed = false;
	status = EXECUTION_SUCCESS;
	while (list)
	{
		if (list->word && list->word->word)
		{
			status = export_variable(list->word->word, g);
			if (status == EXECUTION_MALLOC_FAILURE)
				return (EXECUTION_MALLOC_FAILURE);
			if (status == EXECUTION_FAILURE)
				any_failed = true;
		}
		list = list->next;
	}
	if (any_failed)
		return (EXECUTION_FAILURE);
	return (EXECUTION_SUCCESS);
}
