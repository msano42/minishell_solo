#ifndef BUILTINS_H
# define BUILTINS_H

# include "command.h"
# include "variables.h"

/* **************************************************************** */
/*	                          Defines	                            */
/* **************************************************************** */

/* Possible return values of builtin functions. */
# define EXECUTION_SUCCESS			0
# define EXECUTION_FAILURE			1
# define EX_BADUSAGE				2
# define EXECUTION_MALLOC_FAILURE 	3
# define EX_USAGE					258

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

int		cd_builtin(t_word_list *list, t_globals *g);
int		echo_builtin(t_word_list *list, t_globals *g);
int		exit_builtin(t_word_list *list, t_globals *g);
int		export_builtin(t_word_list *list, t_globals *g);
int		pwd_builtin(t_word_list *list, t_globals *g);
int		unset_builtin(t_word_list *list, t_globals *g);

#endif /* !BUILTINS_H */
