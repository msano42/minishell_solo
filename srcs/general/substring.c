#include "wrappers.h"
#include "general.h"

#include <string.h> // ft_memmove

/* Doesn't include end. */
char	*substring(const char *string, size_t start, size_t end)
{
	char	*result;
	size_t	len;

	if (!string)
		return (NULL);
	if (end < start || end > ft_strlen(string))
		return (NULL);
	len = end - start;
	result = xmalloc(len + 1);
	if (!result)
		return (NULL);
	ft_memmove(result, string + start, len);
	result[len] = '\0';
	return (result);
}
