#include <stddef.h>

#include "command.h"
#include "general.h"
#include "wrappers.h" // memory

static char		**word_list_to_str_array(t_word_list *list);
static void		free_str_array(char **array);
static size_t	word_list_len(t_word_list *list);

char	**make_argv(t_word_list *list)
{
	return (word_list_to_str_array(list));
}

static char	**word_list_to_str_array(t_word_list *list)
{
	size_t	i;
	char	**array;
	size_t	list_len;

	list_len = word_list_len(list);
	array = xmalloc(sizeof(char *) * (list_len + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (list)
	{
		array[i] = ft_strdup(list->word->word);
		if (!array[i])
		{
			free_str_array(array);
			return (NULL);
		}
		i++;
		list = list->next;
	}
	array[i] = NULL;
	return (array);
}

static void	free_str_array(char **array)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static size_t	word_list_len(t_word_list *list)
{
	size_t	len;

	len = 0;
	while (list)
	{
		len++;
		list = list->next;
	}
	return (len);
}
