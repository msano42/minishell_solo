#include <stddef.h>

#include "general.h"

char	*extract_single_quoted_section(const char *str, size_t *idx_ptr)
{
	size_t	i;
	char	*ret;

	if (!idx_ptr || *idx_ptr > ft_strlen(str))
		return (NULL);
	i = *idx_ptr;
	while (str[i] && str[i] != '\'')
		i++;
	ret = substring(str, *idx_ptr, i);
	if (str[i])
		i++;
	*idx_ptr = i;
	return (ret);
}
