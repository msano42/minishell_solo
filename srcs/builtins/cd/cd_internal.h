#ifndef CD_INTERNAL_H
# define CD_INTERNAL_H

# include <stdbool.h>

# include "minishell.h"
# include "variables.h"

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

/* is_absolute_path_for_cd.c */
bool			is_absolute_path_for_cd(const char *path);

/* change_to_directory.c */
enum e_status	change_to_directory(const char *dest, t_globals *g);

/* make_absolute_path.c */
char			*make_absolute_path(const char *str, const char *dot_path);

/* update_pwd_oldpwd */
enum e_status	update_pwd_oldpwd(t_globals *g);

#endif /* !CD_INTERNAL_H */