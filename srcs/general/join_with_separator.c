#include <stddef.h>
#include <stdint.h>

#include "general.h"
#include "wrappers.h" // memory

#include <string.h> // ft_memmove()

/*
	Concatenates two strings with an optional separator in between.
	Returns the concatenated string, or NULL if allocation fails.

	If either string is NULL, it is not included in the resulting string.
	If sep is '\0', it is not included in the resulting string.
*/
char	*join_with_separator(const char *s1, const char *s2, char sep)
{
	char			*ret;
	const size_t	sep_offset = (sep != 0);
	const size_t	s1_len = ft_strlen(s1);
	const size_t	s2_len = ft_strlen(s2);

	if ((SIZE_MAX - sep_offset - 1) == s1_len || \
		((SIZE_MAX - s1_len - sep_offset - 1) < s2_len))
		return (NULL);
	ret = xmalloc(s1_len + s2_len + sep_offset + 1);
	if (!ret)
		return (NULL);
	if (s1)
		ft_memmove(ret, s1, s1_len);
	if (sep)
		ret[s1_len] = sep;
	if (s2)
		ft_memmove(&(ret[s1_len + sep_offset]), s2, s2_len);
	ret[s1_len + s2_len + sep_offset] = '\0';
	return (ret);
}
