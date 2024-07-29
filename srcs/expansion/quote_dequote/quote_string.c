#include <stddef.h>

#include "general.h"
#include "expansion.h"
#include "wrappers.h" // memory

/* NB: \177 = CTLNUL */
char	*quote_string(const char *str)
{
	size_t	i;
	size_t	j;
	char	*ret;

	if (!str)
		return (NULL);
	if (!*str)
		return (ft_strdup("\177"));
	i = 0;
	j = 0;
	ret = xmalloc((ft_strlen(str) * 2) + 1);
	if (!ret)
		return (NULL);
	while (str[i])
	{
		ret[j++] = CTLESC;
		ret[j++] = str[i];
		i++;
	}
	ret[j] = '\0';
	return (ret);
}
