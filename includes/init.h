#ifndef INIT_H
# define INIT_H

# include "minishell.h"
# include "input.h"
# include "variables.h"

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

enum e_status	init_minishell(int argc, char **envp, \
								t_input *input, t_globals *g);

#endif /* INIT_H */
