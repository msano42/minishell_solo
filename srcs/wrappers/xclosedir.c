#include <dirent.h>

int	xclosedir(DIR *dirp)
{
	return (closedir(dirp));
}
