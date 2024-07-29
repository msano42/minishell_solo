#include <stddef.h>

#include "minishell.h"
#include "wrappers.h"
#include "general.h"

#include <string.h> // ft_memmove()

static void				canon_path_internal(const char *path, char *start, \
							char *canon_path, enum e_status *status);
static enum e_status	exclude_prev_elem(char **prev_end, \
							char **dotdot, char *canon_path);
static enum e_status	add_elem(char **prev_end, char **curr_elem, \
							const char *start, char *canon_path);
static void				add_dotdot_elem(char **prev_end, char **dotdot, \
							const char *start);

/*
	Unlike Bash:
		- Doesn't handle CYGWIN.
		- Doesn't take a flag argument. PATH_CHECKDOTDOT and PATH_CHECKEXISTS
		are always assumed to be true.
*/
char	*canonicalize_path(char *path, enum e_status *status)
{
	char	*canon_path;
	char	*start;

	if (!path || !status)
	{
		if (status)
			*status = E_VALUE;
		return (NULL);
	}
	*status = SUCCESS;
	if (!*path)
		path = ".";
	canon_path = ft_strdup(path);
	if (!canon_path)
	{
		*status = E_MALLOC;
		return (NULL);
	}
	start = canon_path + (*path == '/') + is_double_slash_elem(path);
	canon_path_internal(path, start, canon_path, status);
	if (*status == E_CANON)
		return (free(canon_path), NULL);
	if (is_double_slash_elem(canon_path) && !is_double_slash_elem(path))
		ft_memmove(canon_path, canon_path + 1, ft_strlen(canon_path + 1) + 1);
	return (canon_path);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static void	canon_path_internal(const char *path, char *start, \
								char *canon_path, enum e_status *status)
{
	char	*dotdot;
	char	*prev_end;
	char	*curr_elem;

	dotdot = start;
	prev_end = start;
	curr_elem = start;
	while (*curr_elem && *status == SUCCESS)
	{
		if (is_slash_or_dotsep_elem(curr_elem))
			curr_elem++;
		else if (is_dotdot_elem(curr_elem))
		{
			curr_elem += 2;
			if (prev_end > dotdot)
				*status = exclude_prev_elem(&prev_end, &dotdot, canon_path);
			else if (path[0] != '/')
				add_dotdot_elem(&prev_end, &dotdot, start);
		}
		else
			*status = add_elem(&prev_end, &curr_elem, start, canon_path);
	}
	if (prev_end == canon_path && *prev_end)
		*prev_end++ = ((path[0] != '/') * '.') + ((path[0] == '/') * '/');
	*prev_end = '\0';
}

static enum e_status	exclude_prev_elem(char **prev_end, \
											char **dotdot, char *canon_path)
{
	char	tmp_char;

	tmp_char = **prev_end;
	**prev_end = '\0';
	if (!is_directory(canon_path))
		return (E_CANON);
	**prev_end = tmp_char;
	while (--(*prev_end) > *dotdot && !is_path_sep_elem(**prev_end))
		;
	return (SUCCESS);
}

static enum e_status	add_elem(char **prev_end, char **curr_elem, \
								const char *start, char *canon_path)
{
	char	tmp_char;

	if (*prev_end != start)
	{
		**prev_end = '/';
		(*prev_end)++;
	}
	while (**curr_elem && !is_path_sep_elem(**curr_elem))
	{
		**prev_end = **curr_elem;
		(*prev_end)++;
		(*curr_elem)++;
	}
	tmp_char = **prev_end;
	**prev_end = '\0';
	if (!is_directory(canon_path))
		return (E_CANON);
	**prev_end = tmp_char;
	return (SUCCESS);
}

static void	add_dotdot_elem(char **prev_end, char **dotdot, const char *start)
{
	if (*prev_end != start)
	{
		**prev_end = '/';
		(*prev_end)++;
	}
	(*prev_end)[0] = '.';
	(*prev_end)[1] = '.';
	(*prev_end) += 2;
	*dotdot = *prev_end;
}
