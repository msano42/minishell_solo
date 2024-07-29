#ifndef JOBS_H
# define JOBS_H

# include <sys/types.h>
# include <stdint.h>

/* Values for pid_list.flags */
# define PROC_RUNNING		0x01
# define PROC_NOTIFIED		0x02
# define PROC_ASYNC			0x04
# define PROC_SIGNALED		0x10

/* Return values from find_status_by_pid */
# define PROC_BAD			-1
# define PROC_STILL_ALIVE	-2

typedef struct s_pid_list
{
	pid_t				pid;
	int					status;
	uint8_t				flags;
	struct s_pid_list	*next;
}	t_pid_list;

int			get_terminating_signal(int status);
int			get_process_exit_status(int status);

t_pid_list	*add_pid_node(pid_t pid, t_pid_list **list);
t_pid_list	*find_node_by_pid(pid_t pid, t_pid_list *list);
void		set_pid_exit_status(pid_t pid, int status, t_pid_list *list);

int			find_status_by_pid(pid_t pid, t_pid_list *list);
int			find_termsig_by_pid(pid_t pid, t_pid_list *list);

void		reap_zombie_children(t_pid_list *list);

#endif /* !JOBS_H */