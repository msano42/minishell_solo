#include "expansion.h"
#include "command.h"

void	dequote_list(t_word_list *list)
{
	while (list)
	{
		if (list->word && list->word->word)
		{
			if (is_quoted_null(list->word->word))
				list->word->flags &= ~W_HASQUOTEDNULL;
			dequote_string_in_place(list->word->word);
		}
		list = list->next;
	}
}
