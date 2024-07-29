#include "hashlib.h"
#include "variables.h"
#include "wrappers.h" // memory

#include <stddef.h>

static size_t	count_matching_items(t_var_selection_func *func, \
				t_hash_table *table);

void	**make_variable_array(t_var_selection_func *func, t_hash_table *table)
{
	t_bucket	*item;
	void		**array;
	int			table_idx;
	size_t		array_idx;

	array = xmalloc((count_matching_items(func, table) + 1) \
					* sizeof(t_shell_var *));
	if (!array)
		return (NULL);
	table_idx = 0;
	array_idx = 0;
	while (table_idx < table->nbuckets)
	{
		item = table->bucket_array[table_idx];
		while (item)
		{
			if (!func || func(item->data))
				array[array_idx++] = item->data;
			item = item->next;
		}
		table_idx++;
	}
	array[array_idx] = NULL;
	return (array);
}

static size_t	count_matching_items(t_var_selection_func *func, \
				t_hash_table *table)
{
	int			i;
	size_t		cnt;
	t_bucket	*item;

	i = 0;
	cnt = 0;
	while (i < table->nbuckets)
	{
		item = table->bucket_array[i];
		while (item)
		{
			if (!func || func(item->data))
				cnt++;
			item = item->next;
		}
		i++;
	}
	return (cnt);
}
