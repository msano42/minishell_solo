#ifndef SIG_H
# define SIG_H

# include <signal.h>
# include <stdbool.h>

# include "variables.h"

/* **************************************************************** */
/*	                          Defines	                            */
/* **************************************************************** */

/** Default value for g_signal. */
# define NO_SIGNAL 0

/** Typedef for struct sigaction's sa_handler prototype. */
typedef void					t_sig_handler(int);

/** The received signal. */
extern volatile sig_atomic_t	g_signal;

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

/* prompt_signals.c */
void			init_prompt_sighandlers(bool interactive);
int				check_prompt_signals(t_globals *g);

/* set_signal_handler.c */
t_sig_handler	*set_signal_handler(int sig, t_sig_handler *handler);

void	init_parent_sighandlers(void);
void	init_child_sighandlers(void);

#endif /* SIG_H */
