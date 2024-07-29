#include "minishell.h"
#include "general.h"
#include "wrappers.h"

static void	set_skip_ctlxxx(bool *skip_ctlesc, bool *skip_ctlnul, \
			const char *ifs_value);

char	*quote_escapes(const char *str, const char *ifs_value)
{
	size_t		i;
	char		*result;
	bool		skip_ctlesc;
	bool		skip_ctlnul;
	const bool	quote_spaces = (ifs_value && !*ifs_value);

	if (!str)
		return (NULL);
	result = xmalloc((ft_strlen(str) * 2) + 1);
	if (!result)
		return (NULL);
	set_skip_ctlxxx(&skip_ctlesc, &skip_ctlnul, ifs_value);
	i = 0;
	while (*str)
	{
		if ((!skip_ctlesc && *str == CTLESC) \
		|| (!skip_ctlnul && *str == CTLNUL) \
		|| (quote_spaces && *str == ' '))
			result[i++] = CTLESC;
		result[i++] = *str++;
	}
	result[i] = '\0';
	return (result);
}

static void	set_skip_ctlxxx(bool *skip_ctlesc, bool *skip_ctlnul, \
			const char *ifs_value)
{
	*skip_ctlesc = false;
	*skip_ctlnul = false;
	while (ifs_value && *ifs_value)
	{
		*skip_ctlesc |= (*ifs_value == CTLESC);
		*skip_ctlnul |= (*ifs_value == CTLNUL);
		ifs_value++;
	}
}
