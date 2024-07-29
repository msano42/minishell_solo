#ifdef UNIT_TESTING
# include "unity_memory.h"
#else
# include <stdlib.h>
#endif /* UNIT_TESTING */

void	xfree(void *ptr)
{
	free(ptr);
}
