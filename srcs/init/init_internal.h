#ifndef INIT_INTERNAL_H
# define INIT_INTERNAL_H

# include "parser.h"

/* import_environment.c */
enum e_status	import_environment(char **envp, t_globals *g);

/* init_global_variables.c */
void			init_global_variables(t_globals *g);

/* init_input.c */
void			init_input(t_input *input, t_globals *g);

/* init_pwd_oldpwd.c */
enum e_status	init_pwd(t_globals *g);
enum e_status	init_oldpwd(t_globals *g);

/* init_shell_variables.c */
enum e_status	init_shell_variables(char **envp, t_globals *g);

#endif
