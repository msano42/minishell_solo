#include <stdbool.h>

#include "general.h"

bool	is_blank_char(int c)
{
	return (c == ' ' || c == '\t');
}

/* Do we count '\0' as meta? */
bool	is_meta_char(int c)
{
	return (is_blank_char(c) || !!ft_strchr("\n|&;()<>", c));
}

bool	is_quote_char(int c)
{
	return (c == '\"' || c == '\'');
}
