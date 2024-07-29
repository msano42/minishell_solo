#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "variables.h"
#include "general.h"
#include "execute.h"
#include "minishell.h"
#include "jobs.h"
#include "wrappers.h" // memory

t_pid_list	*add_pid(pid_t pid, t_pid_list **list);

pid_t	make_child(t_globals *g)
{
	pid_t	pid;

	g->already_making_children = true;
	pid = fork();
	if (pid < 0)
	{
		dprintf(STDERR_FILENO, "minishell: fork: %s\n", strerror(errno));
		g->last_command_exit_value = EX_NOEXEC;
		return (pid);
	}
	else if (pid != 0)
	{
		g->last_made_pid = pid;
		add_pid_node(pid, &g->pid_list);
	}
	return (pid);
}
