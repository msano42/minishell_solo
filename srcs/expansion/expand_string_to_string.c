#include "expansion.h"

char	*expand_string_to_string(const char *str, int quoted, \
		enum e_status *status, t_globals *g)
{
	t_expansion	exp;
	char		*ret;
	t_word		word;
	t_word_list	*expanded_list;
	t_word_list	*split_list;

	if (!str || !*str)
		return (NULL);
	word.word = (char *)str;
	word.flags = 0;
	init_expansion_struct(&exp, quoted, true);
	expanded_list = expand_word(&word, &exp, g);
	if (exp.status == E_MALLOC)
		*status = E_MALLOC;
	if (!expanded_list || *status == E_MALLOC)
		return (NULL);
	split_list = expanded_list;
	dequote_list(split_list);
	ret = string_list(split_list);
	*status = (!ret * E_MALLOC);
	destroy_word_list(split_list);
	return (ret);
}

	// split_list = word_list_split(expanded_list, status);
	// destroy_word_list(expanded_list);
	// if (!split_list || *status == E_MALLOC)
	// 	return (NULL);