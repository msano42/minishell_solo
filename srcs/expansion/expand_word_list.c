#include "expansion.h"

static t_word_list	*expand_word_list_internal(t_word_list *list, \
					enum e_status *status, t_globals *g);
static t_word_list	*call_expand_word(t_word *word, enum e_status *status, \
					t_globals *g);

void	word_list_append(t_word_list **list, t_word_list *new)
{
	t_word_list	*last;

	if (!list)
		return ;
	if (!*list)
	{
		*list = new;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new;
}

t_word_list	*expand_word_list(t_word_list *list, enum e_status *status, \
			t_globals *g)
{
	t_word_list	*expanded_list;

	*status = SUCCESS;
	if (!list)
		return (NULL);
	expanded_list = expand_word_list_internal(list, status, g);
	if (expanded_list)
		dequote_list(expanded_list);
	destroy_word_list(list);
	return (expanded_list);
}

static t_word_list	*expand_word_list_internal(t_word_list *list, \
					enum e_status *status, t_globals *g)
{
	t_word_list	*result;
	t_word_list	*expanded_word;

	result = NULL;
	while (list)
	{
		expanded_word = call_expand_word(list->word, status, g);
		if (*status == E_MALLOC)
		{
			destroy_word_list(result);
			return (NULL);
		}
		word_list_append(&result, expanded_word);
		list = list->next;
	}
	return (result);
}

static t_word_list	*call_expand_word(t_word *word, enum e_status *status, \
					t_globals *g)
{
	t_expansion	exp;
	t_word_list	*expanded;
	t_word_list	*final_list;

	if (!word)
		return (NULL);
	init_expansion_struct(&exp, 0, false);
	expanded = expand_word(word, &exp, g);
	if (exp.status == E_MALLOC)
	{
		*status = E_MALLOC;
		return (NULL);
	}
	final_list = expanded;
	list_remove_quoted_nulls(final_list);
	return (final_list);
}

// if (exp.expanded_something && !(word->flags & W_NOSPLIT))
// {
// 	final_list = word_list_split(expanded, status);
// 	destroy_word_list(expanded);
// }
// else
// {
// 	final_list = expanded;
// 	list_remove_quoted_nulls(final_list);
// }