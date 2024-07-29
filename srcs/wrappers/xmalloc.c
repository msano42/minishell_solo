#ifdef UNIT_TESTING
# include "unity_memory.h"
#else
# include <stdlib.h>
#endif /* UNIT_TESTING */

void	*xmalloc(size_t size)
{
	return (malloc(size));
}
