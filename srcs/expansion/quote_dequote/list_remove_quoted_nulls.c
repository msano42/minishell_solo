#include "expansion.h"
#include "command.h"

void	list_remove_quoted_nulls(t_word_list *list)
{
	while (list)
	{
		if (list->word && list->word->word)
		{
			remove_quoted_nulls_in_place(list->word->word);
			list->word->flags &= ~W_HASQUOTEDNULL;
		}
		list = list->next;
	}
}
