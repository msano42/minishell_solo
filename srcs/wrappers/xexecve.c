#include <unistd.h>

int	xexecve(const char *path, char *const argv[], char *const envp[])
{
	return (execve(path, argv, envp));
}
