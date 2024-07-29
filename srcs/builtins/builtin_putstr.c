#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "minishell.h"
#include "general.h"

#define BUILTTIN_WRITE_ERR_FORMAT "minishell: %s: write error: %s\n"

enum e_status	builtin_putstr(const char *str, const char *builtin)
{
	if (!str)
		return (SUCCESS);
	if (ft_putstr_fd(str, STDOUT_FILENO) == -1)
	{
		ft_dprintf(STDERR_FILENO, BUILTTIN_WRITE_ERR_FORMAT, \
				builtin, strerror(errno));
		return (E_WRITE);
	}
	return (SUCCESS);
}

enum e_status	builtin_putstr_nl(const char *str, const char *builtin)
{
	if (!str)
		return (SUCCESS);
	if (builtin_putstr(str, builtin) == E_WRITE)
		return (E_WRITE);
	if (builtin_putstr("\n", builtin) == E_WRITE)
		return (E_WRITE);
	return (SUCCESS);
}

enum e_status	builtin_put_assign_nl(const char *name, const char *value, \
				const char *builtin)
{
	if (builtin_putstr(name, builtin) == E_WRITE)
		return (E_WRITE);
	if (builtin_putstr("=", builtin) == E_WRITE)
		return (E_WRITE);
	if (builtin_putstr(value, builtin) == E_WRITE)
		return (E_WRITE);
	if (builtin_putstr("\n", builtin) == E_WRITE)
		return (E_WRITE);
	return (SUCCESS);
}
