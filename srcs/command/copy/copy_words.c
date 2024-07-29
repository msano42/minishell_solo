#include <stddef.h>

#include "command.h"

t_word	*copy_word(t_word *word)
{
	t_word	*new_word;

	if (!word)
		return (NULL);
	new_word = make_bare_word(word->word);
	if (!new_word)
		return (NULL);
	new_word->flags = word->flags;
	return (new_word);
}
