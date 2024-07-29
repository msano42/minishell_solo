#ifndef EXECUTE_H
# define EXECUTE_H

# include "command.h"
# include "variables.h"

#define NO_PIPE -1
#define NO_PID -1

/* Possible values for subshell_environment */
#define SUBSHELL_ASYNC	0x01	/* subshell caused by `command &' */
#define SUBSHELL_PAREN	0x02	/* subshell caused by ( ... ) */
#define SUBSHELL_COMSUB	0x04	/* subshell caused by `command` or $(command) */
#define SUBSHELL_FORK	0x08	/* subshell caused by executing a disk command */
#define SUBSHELL_PIPE	0x10	/* subshell from a pipeline element */
#define SUBSHELL_PROCSUB 0x20	/* subshell caused by <(command) or >(command) */
#define SUBSHELL_COPROC	0x40	/* subshell from a coproc pipeline */
#define SUBSHELL_RESETTRAP 0x80	/* subshell needs to reset trap strings on first call to trap */
#define SUBSHELL_IGNTRAP 0x100  /* subshell should reset trapped signals from trap_handler */

typedef struct s_pipes
{
	int	in;
	int	out;
}	t_pipes;

char	**make_envp(t_hash_table *table);

int	execute_command(t_command *command, t_globals *g);
char	*search_for_command(const char *path_name, enum e_status *status, \
		t_globals *g);
int	execute_disk_command(t_simple_command *simple_command, t_pipes pipes, char *fds_to_close, t_globals *g);

pid_t	make_child(t_globals *g);
void	close_pipes(t_pipes pipes);
char	**make_argv(t_word_list *list);
void	close_fd_bitmap(char *bitmap);
void	do_piping(t_pipes pipes);

#endif /* !EXECUTE_H */
