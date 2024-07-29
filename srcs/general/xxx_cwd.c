#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "minishell.h"
#include "general.h"
#include "wrappers.h" // memory

#define GCWD_ERRSTR "%s: error retrieving current directory: \
getcwd: cannot access parent directories: %s\n"

char	*update_cwd(const char *for_whom, t_globals *g)
{
	free(g->current_working_directory);
	g->current_working_directory = xgetcwd(0, PATH_MAX);
	if (!g->current_working_directory)
	{
		if (!for_whom || !*for_whom)
			for_whom = "minishell";
		ft_dprintf(STDERR_FILENO, GCWD_ERRSTR, for_whom, strerror(errno));
	}
	return (g->current_working_directory);
}

char	*set_cwd(char *cwd, t_globals *g)
{
	free(g->current_working_directory);
	g->current_working_directory = cwd;
	return (g->current_working_directory);
}

// char	*get_working_directory(const char *for_whom, enum e_status *status,
// 								t_globals *g)
// {
// 	char	*cwd;

// 	if (status)
// 		*status = SUCCESS;
// 	if (!g->current_working_directory)
// 	{
// 		g->current_working_directory = xgetcwd(0, PATH_MAX);
// 		if (!g->current_working_directory)
// 		{
// 			if (status)
// 				*status = E_GETCWD;
// 			ft_dprintf(STDERR_FILENO, GCWD_ERRSTR, for_whom, strerror(errno));
// 			return (NULL);
// 		}
// 	}
// 	cwd = ft_strdup(g->current_working_directory);
// 	if (!cwd && status)
// 		*status = E_MALLOC;
// 	return (cwd);
// }

// enum e_status	set_working_directory(const char *cwd, t_globals *g)
// {
// 	free(g->current_working_directory);
// 	g->current_working_directory = NULL;
// 	if (cwd)
// 	{
// 		g->current_working_directory = ft_strdup(cwd);
// 		if (!g->current_working_directory)
// 			return (E_MALLOC);
// 	}
// 	return (SUCCESS);
// }
