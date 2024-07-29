#include "jobs.h"
#include "wrappers.h" // memory

#include <sys/wait.h>
#include <stddef.h>

t_pid_list	*add_pid_node(pid_t pid, t_pid_list **list)
{
	t_pid_list	*new;

	if (!list)
		return (NULL);
	new = xmalloc(sizeof(t_pid_list));
	if (!new)
		return (NULL);
	new->pid = pid;
	new->status = -1;
	new->flags = PROC_RUNNING;
	new->next = NULL;
	if (!*list)
	{
		*list = new;
		return (new);
	}
	new->next = *list;
	return (new);
}

int	find_status_by_pid(pid_t pid, t_pid_list *list)
{
	t_pid_list	*node;

	node = find_node_by_pid(pid, list);
	if (node == NULL)
		return (PROC_BAD);
	if (node->flags & PROC_RUNNING)
		return (PROC_STILL_ALIVE);
	return (node->status);
}

int	get_terminating_signal(int status)
{
	if (WIFSTOPPED(status) == 0 && WIFSIGNALED(status))
		return (WTERMSIG(status));
	return (0);
}

int	find_termsig_by_pid(pid_t pid, t_pid_list *list)
{
	t_pid_list	*node;

	node = find_node_by_pid(pid, list);
	if (!node)
		return (0);
	if (node->flags & PROC_RUNNING)
		return (0);
	return (get_terminating_signal(node->status));
}

t_pid_list	*find_node_by_pid(pid_t pid, t_pid_list *list)
{
	while (list)
	{
		if (list->pid == pid)
			return (list);
		list = list->next;
	}
	return (NULL);
}

int	get_process_exit_status(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS (status));
}

void	set_pid_exit_status(pid_t pid, int status, t_pid_list *list)
{
	t_pid_list	*node;

	node = find_node_by_pid(pid, list);
	if (!node)
		return ;
	node->status = get_process_exit_status(status);
	node->flags &= ~PROC_RUNNING;
	if (WIFSIGNALED(status))
		node->flags |= PROC_SIGNALED;
	// pid_list[slot].flags |= PROC_NOTIFIED;
}

void	reap_zombie_children(t_pid_list *list)
{
	pid_t	pid;
	int		status;

	while (1)
	{
		pid = waitpid(-1, &status, WNOHANG);
		if (pid <= 0)
			break ;
		set_pid_exit_status(pid, status, list);
	}
}
