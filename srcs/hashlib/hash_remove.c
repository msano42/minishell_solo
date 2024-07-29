#include "hashlib.h"
#include "general.h"

static void	remove_entry(t_hash_table *table, t_bucket *current, \
			t_bucket *prev, int idx);

/*
	Does NOT free the content of the removed item.
*/
t_bucket	*hash_remove(const char *key, t_hash_table *table)
{
	int				bucket;
	t_bucket		*prev;
	t_bucket		*current;
	unsigned int	hash_value;

	if (!key || !table || !hash_size(table))
		return (NULL);
	bucket = hash_bucket(key, table);
	hash_value = hash_string(key);
	prev = NULL;
	current = table->bucket_array[bucket];
	while (current)
	{
		if (hash_value == current->key_hash && !ft_strcmp(current->key, key))
		{
			remove_entry(table, current, prev, bucket);
			return (current);
		}
		prev = current;
		current = current->next;
	}
	return (NULL);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

/* Made for norminette. */
static void	remove_entry(t_hash_table *table, t_bucket *current, \
			t_bucket *prev, int idx)
{
	if (prev)
		prev->next = current->next;
	else
		table->bucket_array[idx] = current->next;
	table->nentries--;
}
