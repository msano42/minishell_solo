#include <termios.h>
#include <unistd.h>

#include "variables.h"

#define TCGETATTR_SUCCESS 0

void	get_tty_state(t_globals *g)
{
	if (tcgetattr(STDERR_FILENO, &(g->shell_tty_info)) == TCGETATTR_SUCCESS)
		g->got_tty_state = true;
}

void	set_tty_state(t_globals *g)
{
	if (!g->got_tty_state)
		return ;
	tcsetattr(STDERR_FILENO, TCSADRAIN, &(g->shell_tty_info));
}
