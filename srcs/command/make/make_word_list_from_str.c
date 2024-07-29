#include <stddef.h>

#include "command.h"
#include "general.h"

t_word_list	*make_word_list_from_str(const char *str)
{
	t_word_list	*new;
	t_word		*word;

	word = alloc_word();
	if (!word)
		return (NULL);
	if (str)
		word->word = ft_strdup(str);
	if (str && !word->word)
	{
		destroy_word(word);
		return (NULL);
	}
	new = make_word_list(word, NULL);
	if (!new)
	{
		destroy_word(word);
		return (NULL);
	}
	return (new);
}
