#include <termios.h>

int	xtcgetattr(int fd, struct termios *termios_p)
{
	return (tcgetattr(fd, termios_p));
}
