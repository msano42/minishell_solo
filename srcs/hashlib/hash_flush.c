#include <stddef.h>

#include "hashlib.h"
#include "wrappers.h" // memory

/* 
	If FREE_FUNC is NULL, free() is used to free bucket->data.
*/
void	hash_flush(t_hash_table *table, void (*free_func)(void *))
{
	int			i;
	t_bucket	*item;
	t_bucket	*bucket;

	if (!table || !hash_size(table))
		return ;
	i = 0;
	while (i < table->nbuckets)
	{
		bucket = table->bucket_array[i];
		while (bucket)
		{
			item = bucket;
			bucket = bucket->next;
			if (free_func)
				free_func(item->data);
			else
				free(item->data);
			free(item->key);
			free(item);
		}
		table->bucket_array[i] = NULL;
		i++;
	}
	table->nentries = 0;
}
