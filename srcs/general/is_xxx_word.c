#include <stdbool.h>

#include "command.h"

bool	is_quoted_word(t_word *word)
{
	if (!word)
		return (false);
	return ((word->flags & W_QUOTED) != 0);
}
