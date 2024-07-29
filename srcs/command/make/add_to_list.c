#include <stddef.h>

#include "minishell.h"
#include "command.h"
#include "wrappers.h" // memory

enum e_status	add_to_word_list(t_word *word, t_word_list **list)
{
	t_word_list	*new;
	t_word_list	*end;

	if (!list)
		return (E_VALUE);
	new = xmalloc(sizeof(t_word_list));
	if (!new)
		return (E_MALLOC);
	new->word = word;
	new->next = NULL;
	if (!*list)
	{
		*list = new;
		return (SUCCESS);
	}
	end = *list;
	while (end->next)
		end = end->next;
	end->next = new;
	return (SUCCESS);
}

void	add_to_redirect_list(t_redirect *redirect, t_redirect **list)
{
	t_redirect	*end;

	if (!list)
		return ;
	if (!*list)
	{
		*list = redirect;
		return ;
	}
	end = *list;
	while (end->next)
		end = end->next;
	end->next = redirect;
}
