#include <stddef.h>

#include "hashlib.h"
#include "general.h"

t_bucket	*hash_search(const char *key, t_hash_table *table)
{
	t_bucket		*list;
	int				bucket;
	unsigned int	hash_value;

	if (!key || !table || !hash_size(table))
		return (NULL);
	bucket = hash_bucket(key, table);
	hash_value = hash_string(key);
	list = NULL;
	if (table->bucket_array)
		list = table->bucket_array[bucket];
	while (list)
	{
		if (hash_value == list->key_hash && !ft_strcmp(list->key, key))
		{
			list->times_found++;
			return (list);
		}
		list = list->next;
	}
	return (NULL);
}
