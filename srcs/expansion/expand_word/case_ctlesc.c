#include "expansion.h"

void	case_ctlesc(const char *str, t_expansion *exp)
{
	const char	buf[3] = {CTLESC, str[++exp->idx], '\0'};

	add_string_to_expansion(buf, exp);
	if (str[exp->idx])
		exp->idx++;
}
