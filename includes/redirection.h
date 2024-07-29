#ifndef REDIRECTION_H
# define REDIRECTION_H

# include <stdint.h>

# include "command.h"
# include "variables.h"

/* **************************************************************** */
/*	                          Defines	                            */
/* **************************************************************** */

# define RX_UNDOABLE	0x01 /* backup io fds so they can be restored */

/*
	do_redirections() returns one of these values or errno.
	These MUST BE negative to avoid overlap.
*/
# define AMBIGUOUS_REDIRECT_ERROR	-1 /* filename expansion result was NULL */
# define HEREDOC_REDIRECT_ERROR		-2 /* error creating temp fd for heredoc */
# define MALLOC_REDIRECT_ERROR		-3 /* allocation error during redirection */

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

/* do_redirection.c */
int		do_redirections(t_redirect *list, uint8_t flags, t_globals *g);

/* has_stdin_redirects.c */
bool	has_stdin_redirects(t_redirect *list);

#endif /* !REDIRECTION_H */
