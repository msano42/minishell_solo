#include <stdbool.h>
#include <limits.h> // INT_MAX

#include "minishell.h"
#include "hashlib.h"
#include "wrappers.h" // memory

#define HASH_REHASH_FACTOR 2
#define HASH_REHASH_MULTIPLIER 4

static bool				hash_should_grow(t_hash_table *table);
static enum e_status	hash_grow(t_hash_table *table);
static enum e_status	hash_rehash(t_hash_table *table, int new_size);
static void				rehash_array(t_bucket **new_array, \
						t_bucket **old_array, int new_size, int old_size);

t_bucket	*hash_insert(char *key, t_hash_table *table, uint8_t flags)
{
	t_bucket		*item;
	int				bucket;

	if (!key || !table)
		return (NULL);
	item = NULL;
	if (!(flags & HASH_NOSRCH))
		item = hash_search(key, table);
	if (!item)
	{
		if (hash_should_grow(table) && hash_grow(table) != SUCCESS)
			return (NULL);
		item = xmalloc(sizeof(t_bucket));
		if (!item)
			return (NULL);
		bucket = hash_bucket(key, table);
		item->next = table->bucket_array[bucket];
		table->bucket_array[bucket] = item;
		item->data = NULL;
		item->key = key;
		item->key_hash = hash_string(key);
		item->times_found = 0;
		table->nentries++;
	}
	return (item);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

/*
   Checks if the hash table should be resized for more entries.
*/
static bool	hash_should_grow(t_hash_table *table)
{
	if (!table)
		return (false);
	if (table->nbuckets > (INT_MAX / HASH_REHASH_FACTOR))
		return (false);
	return (table->nentries >= (table->nbuckets * HASH_REHASH_FACTOR));
}

/*
	Increases the size of the bucket_array by factor of HASH_REHASH_MULTIPLIER.

	Returns E_OVERFLOW if the new_size exceeds INT_MAX.
*/
static enum e_status	hash_grow(t_hash_table *table)
{
	int	new_size;

	if (table->nbuckets > (INT_MAX / HASH_REHASH_MULTIPLIER))
		return (SUCCESS);
	new_size = table->nbuckets * HASH_REHASH_MULTIPLIER;
	return (hash_rehash(table, new_size));
}
	// int	new_size;

	// new_size = table->nbuckets * HASH_REHASH_MULTIPLIER;
	// if (new_size > 0)
	// 	return (hash_rehash(table, new_size));
	// else
	// 	return (SUCCESS);

/*
	Reallocates table->bucket_array with new_new size and rehashes items to
	the proper index. Updates table->nbuckets to new_size.

	Returns E_MALLOC on failure without freeing table->bucket_array.

	Otherwise table->bucket_array is freed.
*/
static enum e_status	hash_rehash(t_hash_table *table, int new_size)
{
	int			i;
	t_bucket	**new_array;

	if (!table || new_size == table->nbuckets)
		return (SUCCESS);
	new_array = xmalloc(new_size * sizeof(t_bucket *));
	if (!new_array)
		return (E_MALLOC);
	i = 0;
	while (i < new_size)
		new_array[i++] = NULL;
	rehash_array(new_array, table->bucket_array, new_size, table->nbuckets);
	free(table->bucket_array);
	table->nbuckets = new_size;
	table->bucket_array = new_array;
	return (SUCCESS);
}

/*
	Rehashes items to the proper index.

	NB: Just made this function for norminette.
*/
static void	rehash_array(t_bucket **new_array, \
			t_bucket **old_array, int new_size, int old_size)
{
	t_bucket	*item;
	t_bucket	*next;
	int			old_index;
	int			new_index;

	old_index = 0;
	new_index = 0;
	while (old_index < old_size)
	{
		item = old_array[old_index];
		while (item)
		{
			next = item->next;
			new_index = item->key_hash & (new_size - 1);
			item->next = new_array[new_index];
			new_array[new_index] = item;
			item = next;
		}
		old_index++;
	}
}
