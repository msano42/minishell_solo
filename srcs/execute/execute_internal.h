#ifndef EXECUTE_INTERNAL_H
# define EXECUTE_INTERNAL_H

void	close_pipes(t_pipes pipes);

pid_t	make_child(t_globals *g);

int	execute_command_internal(t_command *command, t_pipes pipes, char *fds_to_close, t_globals *g);
int	execute_pipeline(t_command *command, t_pipes pipes, char *fds_to_close, t_globals *g);
int	execute_simple_command(t_simple_command *command, t_pipes pipes, char *fds_to_close, t_globals *g);
int	execute_subshell_command(t_command *command, t_pipes pipes, char *fds_to_close, t_globals *g);
int	execute_in_subshell(t_command *command, t_pipes pipes, char *fds_to_close, t_globals *g);
int	execute_connection(t_command *command, t_pipes pipes, char *fds_to_close, t_globals *g);

int	wait_for(pid_t pid, t_globals *g);

#endif /* !EXECUTE_INTERNAL_H */
