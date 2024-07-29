#include "expansion.h"
#include "general.h"
#include "minishell.h"
#include "wrappers.h" // memory

void	add_string_to_expansion(const char *str, t_expansion *exp)
{
	char	*old_expanded_str;

	if (!str || !*str)
		return ;
	old_expanded_str = exp->expanded_str;
	exp->expanded_str = ft_strjoin(exp->expanded_str, str);
	if (!exp->expanded_str)
		exp->status = E_MALLOC;
	free(old_expanded_str);
}

void	add_char_to_expansion(char c, t_expansion *exp)
{
	const char	buf[2] = {c, '\0'};

	add_string_to_expansion(buf, exp);
}

void	add_quoted_string_to_expansion(const char *str, t_expansion *exp)
{
	char	*quoted_str;

	if (!str)
	{
		add_char_to_expansion(CTLNUL, exp);
		exp->had_quoted_null = true;
		return ;
	}
	quoted_str = quote_string(str);
	if (!quoted_str)
	{
		exp->status = E_MALLOC;
		return ;
	}
	add_string_to_expansion(quoted_str, exp);
	free(quoted_str);
}
