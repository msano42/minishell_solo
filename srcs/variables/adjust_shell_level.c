#include <stdint.h>

#include "variables.h"
#include "minishell.h"
#include "general.h"
#include "wrappers.h" // memory

#define MAX_SHLVL 1000

#define MAX_SHLVL_ERR_FORMAT "minishell: warning: shell level (%d) too high, \
resetting to 1\n"

static intmax_t	add_with_overflow_wrap_around(intmax_t a, intmax_t b);

enum e_status	adjust_shell_level(int change, t_globals *g)
{
	char		*str;
	t_shell_var	*var;
	intmax_t	old_level;

	str = get_string_value("SHLVL", g);
	if (!str || *str == '\0' || !is_legal_number(str, &old_level))
		old_level = 0;
	g->shell_level = old_level + change;
	if (g->shell_level < 0)
		g->shell_level = 0;
	else if (g->shell_level >= MAX_SHLVL)
	{
		ft_dprintf(2, MAX_SHLVL_ERR_FORMAT, g->shell_level);
		g->shell_level = 1;
	}
	str = ft_itoa((int)g->shell_level);
	if (!str)
		return (E_MALLOC);
	var = bind_variable("SHLVL", str, 0, g);
	free(str);
	if (!var)
		return (E_MALLOC);
	var->attributes |= ATT_EXPORTED;
	g->array_needs_making = true;
	return (SUCCESS);
}
