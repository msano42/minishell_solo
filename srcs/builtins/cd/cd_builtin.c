#include <errno.h>
#include <string.h>

#include "general.h"
#include "command.h"
#include "builtins.h"
#include "wrappers.h"
#include "cd_internal.h"
#include "builtins_internal.h"

#define CD_HOME_ERR_STR "minishell: cd: HOME not set\n"
#define CD_OLD_PWD_ERR_STR "minishell: cd: OLDPWD not set\n"
#define CD_TOO_MANY_ERR_STR "minishell: cd: too many arguments\n"
#define CD_ERR_FORMAT "minishell: cd: %s: %s\n"

static char				*get_dirname(t_word_list *list, bool *print_path, \
						t_globals *g);
static bool				should_try_cdpath(const char *cdpath, \
						const char *dirname, t_word_list *list);
static enum e_status	cd_with_cdpath(const char *cdpath, \
						const char *dirname, bool *print_path, t_globals *g);
static int				call_change_to_directory(const char *dirname, \
						bool print_path, t_globals *g);

/* bash 3.2 doesn't complain with too many arguments */

int	cd_builtin(t_word_list *list, t_globals *g)
{
	enum e_status	status;
	bool			print_path;
	char			*dirname;
	const char		*cdpath = get_string_value("CDPATH", g);

	if (check_options(&list, "cd", CD_USAGE, CD_LONGDOC) == EX_BADUSAGE)
		return (EX_BADUSAGE);
	dirname = get_dirname(list, &print_path, g);
	if (!dirname)
		return (EXECUTION_FAILURE);
	if (should_try_cdpath(cdpath, dirname, list))
	{
		status = cd_with_cdpath(cdpath, dirname, &print_path, g);
		if (status == SUCCESS)
		{
			if (print_path && \
				builtin_putstr_nl(g->current_working_directory, "cd"))
				return (EXECUTION_FAILURE);
			return (EXECUTION_SUCCESS);
		}
		if (status == E_MALLOC)
			return (EXECUTION_MALLOC_FAILURE);
	}
	return (call_change_to_directory(dirname, print_path, g));
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static char	*get_dirname(t_word_list *list, bool *print_path, t_globals *g)
{
	char	*dirname;

	*print_path = false;
	if (!list || !list->word || !list->word->word)
	{
		dirname = get_string_value("HOME", g);
		if (!dirname)
			ft_dprintf(STDERR_FILENO, CD_HOME_ERR_STR);
	}
	else if (list->next)
	{
		ft_dprintf(STDERR_FILENO, CD_TOO_MANY_ERR_STR);
		dirname = NULL;
	}
	else if (list->word->word[0] == '-' && list->word->word[1] == '\0')
	{
		dirname = get_string_value("OLDPWD", g);
		if (!dirname)
			ft_dprintf(STDERR_FILENO, CD_OLD_PWD_ERR_STR);
		*print_path = true;
	}
	else
		dirname = list->word->word;
	return (dirname);
}

/*
	(dirname != list->word->word) when using $OLDPWD or $HOME
*/
static bool	should_try_cdpath(const char *cdpath, const char *dirname, \
			t_word_list *list)
{
	if (!list || !list->word || !list->word->word)
		return (false);
	if (dirname != list->word->word)
		return (false);
	return (cdpath && !is_absolute_path_for_cd(dirname));
}

static enum e_status	cd_with_cdpath(const char *cdpath, \
						const char *dirname, bool *print_path, t_globals *g)
{
	enum e_status	status;
	size_t			path_index;
	char			*full_path;
	char			*path_unit;

	path_index = 0;
	status = ERROR;
	while (cdpath[path_index] && status != SUCCESS && status != E_MALLOC)
	{
		path_unit = extract_colon_unit(cdpath, &path_index);
		if (!path_unit)
			return (E_MALLOC);
		full_path = join_path(path_unit, dirname, 0);
		if (!full_path)
			return (free(path_unit), E_MALLOC);
		status = change_to_directory(full_path, g);
		if (status == SUCCESS && path_unit[0] && g->current_working_directory)
			*print_path = true;
		free(path_unit);
		free(full_path);
	}
	if (status == SUCCESS)
		return (update_pwd_oldpwd(g));
	return (status);
}

static int	call_change_to_directory(const char *dirname, bool print_path, \
			t_globals *g)
{
	enum e_status	status;
	char			*printable_dirname;

	status = change_to_directory(dirname, g);
	if (status == E_MALLOC)
		return (EXECUTION_MALLOC_FAILURE);
	if (status == SUCCESS)
	{
		if (update_pwd_oldpwd(g) == E_MALLOC)
			return (EXECUTION_MALLOC_FAILURE);
		if (print_path)
		{
			if (builtin_putstr_nl(dirname, "cd") == E_WRITE)
				return (EXECUTION_FAILURE);
		}
		return (EXECUTION_SUCCESS);
	}
	printable_dirname = printable_filename(dirname);
	if (!printable_dirname)
		return (EXECUTION_MALLOC_FAILURE);
	ft_dprintf(STDERR_FILENO, CD_ERR_FORMAT, printable_dirname, \
				strerror(errno));
	free(printable_dirname);
	return (EXECUTION_FAILURE);
}
