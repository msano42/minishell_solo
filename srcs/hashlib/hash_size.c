#include "hashlib.h"

/*
	Check if table is NULL in the calling function to distinguish a NULL table
	from an empty table.
*/
int	hash_size(t_hash_table *table)
{
	if (!table)
		return (0);
	return (table->nentries);
}
