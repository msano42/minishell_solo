#include "jobs.h"
#include "variables.h"
#include "input.h"

#include <sys/wait.h>
#include <errno.h>

static pid_t	wait_loop(pid_t pid, int *status, t_pid_list *list);

int	wait_for(pid_t pid, t_globals *g)
{
	int		result;
	int		proc_status;
	pid_t	got_pid;
	int		status;

	// proc_status = find_status_by_pid(pid, g->pid_list);
	// if (proc_status == PROC_BAD)
	// 	return (0);
	// if (proc_status != PROC_STILL_ALIVE)
	// 	return (proc_status);
	got_pid = wait_loop(pid, &status, g->pid_list);
	if (got_pid > 0)
		set_pid_exit_status(got_pid, status, g->pid_list);
	if (got_pid >= 0)
		reap_zombie_children(g->pid_list);
	result = get_process_exit_status(status);
	// if (g->interactive && !g->subshell_environment)
	// {
	// 	if (WIFSIGNALED (status))
	// 		set_tty_state(g);
	// 	else
	// 		get_tty_state(g);
	// }
	return (result);
}

static pid_t	wait_loop(pid_t pid, int *status, t_pid_list *list)
{
	pid_t	got_pid;

	while (1)
	{
		got_pid = waitpid(-1, status, 0);
		if (got_pid == pid)
			break ;
		if (got_pid < 0 && errno == ECHILD)
		{
			*status = 0;
			break ;
		}
		else if (got_pid > 0)
			set_pid_exit_status(got_pid, *status, list);
	}
	return (got_pid);
}
