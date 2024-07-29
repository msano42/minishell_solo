#include <stddef.h>

#include "wrappers.h"
#include "general.h"

char	*strfmt_double_quote_and_escape(const char *str)
{
	char	*ret;
	size_t	i;
	size_t	j;

	if (!str)
		return (NULL);
	ret = xmalloc((ft_strlen(str) * 2) + 3);
	if (!ret)
		return (NULL);
	i = 0;
	j = 0;
	ret[j++] = '\"';
	while (str[i])
	{
		if (is_escaped_in_dquotes(str[i]) && str[i] != '\n')
			ret[j++] = '\\';
		ret[j++] = str[i++];
	}
	ret[j++] = '\"';
	ret[j] = '\0';
	return (ret);
}
