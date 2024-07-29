#include <stddef.h>
#include <unistd.h> // access()
#include <sys/stat.h> // stat()

#include "general.h"
#include "wrappers.h" // memory

#define ACCESS_SUCCESS 0

#define FS_EXISTS			0x1
#define FS_EXECABLE			0x2
#define FS_DIRECTORY		0x4
#define FS_READABLE			0x8

static char	*find_user_command_in_path(const char *command, char *path_list, \
			enum e_status *status);
static char	*get_next_path_element(const char *path_list, size_t *index_ptr);
static char	*find_in_path_element(const char *command, const char *path, \
			char **backup_file, enum e_status *status);
static int	get_file_flags(const char *file);

char	*search_for_command(const char *path_name, enum e_status *status, \
		t_globals *g)
{
	char		*command;
	t_shell_var	*path_var;
	char		*path_list;

	*status = SUCCESS;
	if (!path_name)
		return (NULL);
	path_list = NULL;
	path_var = find_variable("PATH", g);
	if (path_var)
		path_list = path_var->value;
	if (!path_list || !*path_list || is_absolute_program(path_name))
	{
		command = ft_strdup(path_name);
		if (!command)
			*status = E_MALLOC;
	}
	else
		command = find_user_command_in_path(path_name, path_list, status);
	return (command);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static char	*find_user_command_in_path(const char *command, char *path_list, \
			enum e_status *status)
{
	char	*element;
	char	*full_path;
	char	*backup_file;
	size_t	path_index;

	path_index = 0;
	backup_file = NULL;
	while (path_list[path_index])
	{
		element = get_next_path_element(path_list, &path_index);
		if (!element)
		{
			*status = E_MALLOC;
			free(backup_file);
			return (NULL);
		}
		full_path = find_in_path_element(command, element, \
					&backup_file, status);
		free(element);
		if (full_path || *status == E_MALLOC)
			return (full_path);
	}
	return (backup_file);
}

static char	*get_next_path_element(const char *path_list, size_t *index_ptr)
{
	char	*element;

	element = extract_colon_unit(path_list, index_ptr);
	if (!element)
		return (NULL);
	if (*element == '\0')
	{
		free(element);
		element = ft_strdup(".");
	}
	return (element);
}

static char	*find_in_path_element(const char *command, const char *element, \
			char **backup_file, enum e_status *status)
{
	int		flgs;
	char	*full_path;

	full_path = join_path(element, command, 0);
	if (!full_path)
	{
		*status = E_MALLOC;
		free(*backup_file);
		*backup_file = NULL;
		return (NULL);
	}
	flgs = get_file_flags(full_path);
	if ((flgs & FS_EXISTS) && (flgs & FS_EXECABLE) && !(flgs & FS_DIRECTORY))
	{
		free(*backup_file);
		*backup_file = NULL;
		return (full_path);
	}
	if (!*backup_file && (flgs & FS_EXISTS) && !(flgs & FS_DIRECTORY))
		*backup_file = full_path;
	else
		free(full_path);
	return (NULL);
}

static int	get_file_flags(const char *file)
{
	int			flags;
	struct stat	info;

	if (!file || stat(file, &info) < 0)
		return (0);
	if (S_ISDIR(info.st_mode))
		return (FS_EXISTS | FS_DIRECTORY);
	flags = FS_EXISTS;
	if (access(file, X_OK) == ACCESS_SUCCESS)
		flags |= FS_EXECABLE;
	if (access(file, R_OK) == ACCESS_SUCCESS)
		flags |= FS_READABLE;
	return (flags);
}
