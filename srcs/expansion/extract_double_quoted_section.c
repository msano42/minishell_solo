#include <stddef.h>
#include <stdbool.h>

#include "general.h"
#include "wrappers.h"

static size_t	double_quoted_len(const char *str, size_t idx);
static void		fill_double_quoted_section(char *buf, const char *str, \
				size_t *idx_ptr);

char	*extract_double_quoted_section(const char *str, size_t *idx_ptr)
{
	char	*ret;

	if (!str || !idx_ptr || *idx_ptr > ft_strlen(str))
		return (NULL);
	ret = xmalloc(double_quoted_len(str, *idx_ptr) + 1);
	if (!ret)
		return (NULL);
	fill_double_quoted_section(ret, str, idx_ptr);
	return (ret);
}

static size_t	double_quoted_len(const char *str, size_t idx)
{
	size_t	len;
	bool	pass_next;

	len = 0;
	pass_next = false;
	while (str[idx])
	{
		if (pass_next)
		{
			if (str[idx] != '"')
				len++;
			len++;
			pass_next = false;
		}
		else if (str[idx] == '"')
			break ;
		else if (str[idx] == '\\')
			pass_next = true;
		else
			len++;
		idx++;
	}
	return (len);
}

static void	fill_double_quoted_section(char *buf, const char *str, \
			size_t *idx_ptr)
{
	size_t	buf_idx;
	bool	pass_next;

	pass_next = false;
	buf_idx = 0;
	while (str[*idx_ptr] && (pass_next || str[*idx_ptr] != '"'))
	{
		if (pass_next)
		{
			if (str[*idx_ptr] != '"')
				buf[buf_idx++] = '\\';
			buf[buf_idx++] = str[(*idx_ptr)++];
			pass_next = false;
		}
		else if (str[*idx_ptr] == '\\')
		{
			pass_next = true;
			(*idx_ptr)++;
		}
		else
			buf[buf_idx++] = str[(*idx_ptr)++];
	}
	buf[buf_idx] = '\0';
	if (str[*idx_ptr])
		(*idx_ptr)++;
}
