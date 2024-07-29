#include <stdbool.h>

#include "expansion.h"
#include "general.h"

#define MAX_ESCAPE_SEQ_BUFSIZE 5

static void	fill_backslash_escape_sequence(char buf[MAX_ESCAPE_SEQ_BUFSIZE], \
			char c, t_expansion *exp, t_globals *g);
static bool	is_escaped(char c, t_expansion *exp);

void	case_backslash(const char *str, t_expansion *exp, t_globals *g)
{
	char	buf[MAX_ESCAPE_SEQ_BUFSIZE];

	exp->idx++;
	if (str[exp->idx] == '\n')
	{
		exp->idx++;
		return ;
	}
	fill_backslash_escape_sequence(buf, str[exp->idx], exp, g);
	if (str[exp->idx])
		exp->idx++;
	add_string_to_expansion(buf, exp);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static void	fill_backslash_escape_sequence(char buf[MAX_ESCAPE_SEQ_BUFSIZE], \
			char c, t_expansion *exp, t_globals *g)
{
	ft_memset(buf, '\0', MAX_ESCAPE_SEQ_BUFSIZE);
	if (in_dquotes_or_heredoc(exp) && !is_escaped(c, exp) && is_ifs(c, g))
	{
		buf[0] = CTLESC;
		buf[1] = '\\';
		buf[2] = CTLESC;
		buf[3] = c;
	}
	else if (in_dquotes_or_heredoc(exp) && c == '\0')
	{
		buf[0] = CTLESC;
		buf[1] = '\\';
	}
	else if (in_dquotes_or_heredoc(exp) && !is_escaped(c, exp))
	{
		buf[0] = '\\';
		buf[1] = c;
	}
	else if (c == '\0')
		buf[0] = CTLNUL;
	else
	{
		buf[0] = CTLESC;
		buf[1] = c;
	}
}

/*
	CBSDQUOTE = "\n\"$\\`"
	CBSHDOC = "$\\`"
	Backquote ('`') is not special in minishell.
*/
static bool	is_escaped(char c, t_expansion *exp)
{
	if (!exp)
		return (false);
	if (exp->in_heredoc)
		return (c == '$' || c == '\\');
	if (exp->in_dquotes) /* replace with is_escaped_in_dquotes() */
		return (c == '\n' || c == '"' || c == '$' || c == '\\');
	return (false);
}
