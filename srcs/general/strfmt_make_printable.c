#include <stddef.h>

#include "general.h"
#include "wrappers.h" // memory

#define ESC '\033'

static size_t	printed_str_len(const char *str);
static size_t	printed_char_len(char c);
static void		insert_printed_char(char *dst, char c);

char	*strfmt_make_printable(const char *str)
{
	size_t	i;
	size_t	j;
	char	*result;

	if (!str)
		return (NULL);
	result = xmalloc(printed_str_len(str) + 3 + 1);
	if (!result)
		return (NULL);
	result[0] = '$';
	result[1] = '\'';
	i = 0;
	j = 2;
	while (str[i])
	{
		insert_printed_char(&result[j], str[i]);
		j += printed_char_len(str[i]);
		i++;
	}
	result[j++] = '\'';
	result[j] = '\0';
	return (result);
}

static size_t	printed_str_len(const char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = 0;
	while (*str)
	{
		len += printed_char_len(*str);
		str++;
	}
	return (len);
}

static size_t	printed_char_len(char c)
{
	if (ft_isprint(c))
		return (1);
	if (c == ESC || \
		c == '\a' || \
		c == '\v' || \
		c == '\b' || \
		c == '\f' || \
		c == '\n' || \
		c == '\r' || \
		c == '\t')
		return (2);
	return (4);
}

static void	insert_printed_char(char *dst, char c)
{
	if (ft_isprint(c))
		dst[0] = c;
	else if (c == ESC)
		ft_memmove(dst, "\\E", 2);
	else if (c == '\a')
		ft_memmove(dst, "\\a", 2);
	else if (c == '\v')
		ft_memmove(dst, "\\v", 2);
	else if (c == '\b')
		ft_memmove(dst, "\\b", 2);
	else if (c == '\f')
		ft_memmove(dst, "\\f", 2);
	else if (c == '\n')
		ft_memmove(dst, "\\n", 2);
	else if (c == '\r')
		ft_memmove(dst, "\\r", 2);
	else if (c == '\t')
		ft_memmove(dst, "\\t", 2);
	else
	{
		dst[0] = '\\';
		dst[1] = ((c >> 6) & 07) + '0';
		dst[2] = ((c >> 3) & 07) + '0';
		dst[3] = (c & 07) + '0';
	}
}
