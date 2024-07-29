#include <string.h>

char	*xstrerror(int errnum)
{
	return (strerror(errnum));
}
