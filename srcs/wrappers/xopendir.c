#include <dirent.h>

DIR	*xopendir(const char *filename)
{
	return (opendir(filename));
}
