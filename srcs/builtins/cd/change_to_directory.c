#include <errno.h>

#include "general.h"
#include "wrappers.h" // memory

#define CHDIR_SUCCESS 0

char					*make_absolute_path(const char *str, \
						const char *dot_path);

static char				*get_chdir_path(const char *dest, \
						enum e_status *status, t_globals *g);
static enum e_status	call_chdir(const char *original_path, \
						char *canon_path, enum e_status canon_status, \
						t_globals *g);

enum e_status	change_to_directory(const char *dest, t_globals *g)
{
	char			*canon_path;
	enum e_status	canon_status;

	canon_status = SUCCESS;
	canon_path = get_chdir_path(dest, &canon_status, g);
	if (canon_status == E_MALLOC)
		return (E_MALLOC);
	if (g->posix && canon_status == E_CANON && \
		(errno != ENAMETOOLONG || ft_strlen(dest) > PATH_MAX))
	{
		if (errno != ENOENT && errno != ENAMETOOLONG)
			errno = ENOTDIR;
		free(canon_path);
		return (E_CANON);
	}
	return (call_chdir(dest, canon_path, canon_status, g));
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static char	*get_chdir_path(const char *dest, enum e_status *status, \
			t_globals *g)
{
	char	*result;
	char	*abs_path;

	if (!g->current_working_directory)
		update_cwd("chdir", g);
	abs_path = make_absolute_path(dest, g->current_working_directory);
	if (!abs_path)
	{
		*status = E_MALLOC;
		return (NULL);
	}
	result = canonicalize_path(abs_path, status);
	if (*status == E_MALLOC)
	{
		free(abs_path);
		return (NULL);
	}
	if (!result || !*result)
	{
		free(result);
		result = abs_path;
	}
	else
		free(abs_path);
	return (result);
}

static enum e_status	call_chdir(const char *original_path, \
						char *canon_path, enum e_status canon_status, \
						t_globals *g)
{
	int	old_errno;

	if (xchdir(canon_path) == CHDIR_SUCCESS)
	{
		if (canon_status == SUCCESS || update_cwd("cd", g) == NULL)
			set_cwd(canon_path, g);
		else
			free(canon_path);
		return (SUCCESS);
	}
	old_errno = errno;
	if (!g->posix && xchdir(original_path) == CHDIR_SUCCESS)
	{
		if (update_cwd("cd", g) == NULL)
			set_cwd(canon_path, g);
		else
			free(canon_path);
		return (SUCCESS);
	}
	free(canon_path);
	errno = old_errno;
	return (E_CHDIR);
}

/* **************************************************************** */
/*	                 Static Functions Documentation	            	*/
/* **************************************************************** */

/**
	@fn static char	*get_chdir_path(char *dest, \
	enum e_status *status, t_globals *g)

	@brief Obtains the canonical path of the specified directory.

	@li An absolute path is obtained using make_absolute_path().
	@li The canonicalized path is obtained by calling canonicalize_path()
	on the absolute path.
	@li If canonicalization fails, the absolute path is returned.

	@param dest The directory to change to.
	@param status A pointer to an enum e_status representing the status
	of the operation.
	@param g Pointer to the global variables struct.

	@return An allocated string containing the canonical path of \ar dest, or
	NULL on allocation failure.

	\ar *status is set according to the execution status:
	@li \ar SUCCESS
	@li \ar E_CANON canonicalization error
	@li \ar E_MALLOC memory allocation failure
*/

/**
	@fn static enum e_status	call_chdir(char *original_path, \
						char *canon_path, enum e_status canon_status, \
						t_globals *g)

	@brief Does the work of changing the directory for change_to_directory().

	@li If chdir(2) fails with \ar canon_path and \ar g->posix is \ar false,
	attempts chdir(2) with \ar original_path.

	@param original_path The argument to cd_builtin().
	@param canon_path The canonical path of \ar original_path.
	@param canon_status The status of the canonicalization process.
	@param g Pointer to the global variables struct.

	@return Execution status:
	@li \ar SUCCESS
	@li \ar E_CHDIR chdir(2) error
*/
