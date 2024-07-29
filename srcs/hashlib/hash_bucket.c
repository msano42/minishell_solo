#include "hashlib.h"

int	hash_bucket(const char *key, t_hash_table *table)
{
	return ((hash_string(key) & (table->nbuckets - 1)));
}
