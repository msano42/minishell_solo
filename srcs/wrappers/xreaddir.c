#include <dirent.h>

struct dirent	*xreaddir(DIR *dirp)
{
	return (readdir(dirp));
}
