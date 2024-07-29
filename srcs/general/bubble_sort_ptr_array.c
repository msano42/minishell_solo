#include <stdbool.h>

#include "general.h"

static bool	ptr_array_is_sorted(void **array, t_comp_func func);

void	bubble_sort_ptr_array(void **array, t_comp_func func)
{
	size_t	i;
	void	*tmp;

	if (!array || !func)
		return ;
	while (!ptr_array_is_sorted(array, func))
	{
		i = 0;
		while (array[i] && array[i + 1])
		{
			if (func(array[i], array[i + 1]) > 0)
			{
				tmp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = tmp;
			}
			i++;
		}
	}
}

static bool	ptr_array_is_sorted(void **array, t_comp_func func)
{
	size_t	i;

	if (!array || !func)
		return (true);
	i = 0;
	while (array[i] && array[i + 1])
	{
		if (func(array[i], array[i + 1]) > 0)
			return (false);
		i++;
	}
	return (true);
}
