#include "hashlib.h"
#include "wrappers.h" // memory

/*
	Does not free the contents of the bucket array.
	Call hash_flush() before calling hash_destroy() to avoid memory leaks.
*/
void	hash_destroy(t_hash_table *table)
{
	if (!table)
		return ;
	free(table->bucket_array);
	free(table);
}
