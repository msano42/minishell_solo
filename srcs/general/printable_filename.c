#include <stddef.h>

#include "general.h"

char	*printable_filename(const char *filename)
{
	if (!filename)
		return (NULL);
	if (has_unprintable_chars(filename))
		return (strfmt_make_printable(filename));
	return (ft_strdup(filename));
}
