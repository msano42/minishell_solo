#include "command.h"
#include "general.h"
#include "wrappers.h"

#include <stddef.h>

static size_t	word_list_combined_len(t_word_list *list, const char *sep);
static char		*string_list_internal(t_word_list *list, const char *sep);

char	*string_list(t_word_list *list)
{
	return (string_list_internal(list, " "));
}

char	*string_list_internal(t_word_list *list, const char *sep)
{
	char	*result;
	char	*position;

	if (!list)
		return (NULL);
	result = xmalloc(word_list_combined_len(list, sep) + 1);
	if (!result)
		return (NULL);
	position = result;
	while (list)
	{
		ft_memmove(position, list->word->word, ft_strlen(list->word->word));
		position += ft_strlen(list->word->word);
		if (!list->next)
			break ;
		ft_memmove(position, sep, ft_strlen(sep));
		list = list->next;
	}
	*position = '\0';
	return (result);
}

static size_t	word_list_combined_len(t_word_list *list, const char *sep)
{
	size_t	sep_len;
	size_t	combined_len;

	sep_len = 0;
	combined_len = 0;
	if (sep)
		sep_len = ft_strlen(sep);
	while (list)
	{
		if (list->next)
			combined_len += sep_len;
		combined_len += ft_strlen(list->word->word);
		list = list->next;
	}
	return (combined_len);
}
