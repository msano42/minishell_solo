#include <stddef.h>
#include <stdbool.h>

#include "hashlib.h"
#include "wrappers.h" // memory

#define DEFAULT_HASH_BUCKETS 128

static bool	_is_power_of_2(int num);

t_hash_table	*hash_create(int num_buckets)
{
	int				i;
	t_hash_table	*table;

	if (num_buckets && !_is_power_of_2(num_buckets))
		return (NULL);
	table = xmalloc(sizeof(t_hash_table));
	if (!table)
		return (NULL);
	if (num_buckets == 0)
		num_buckets = DEFAULT_HASH_BUCKETS;
	table->bucket_array = xmalloc(num_buckets * sizeof(t_bucket *));
	if (!table->bucket_array)
	{
		free(table);
		return (NULL);
	}
	table->nbuckets = num_buckets;
	table->nentries = 0;
	i = 0;
	while (i < num_buckets)
	{
		table->bucket_array[i] = NULL;
		i++;
	}
	return (table);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static bool	_is_power_of_2(int num)
{
	return (!(num & (num - 1)) && num > 0);
}
