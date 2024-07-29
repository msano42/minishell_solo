#include "command.h"
#include "wrappers.h" // memory

void	destroy_word(t_word *word)
{
	if (!word)
		return ;
	free(word->word);
	free(word);
}

void	destroy_word_list(t_word_list *list)
{
	t_word_list	*next;

	while (list)
	{
		next = list->next;
		destroy_word(list->word);
		free(list);
		list = next;
	}
}
