#include <stddef.h>

#include "command.h"
#include "general.h"
#include "wrappers.h" // memory
#include "variables.h"
#include "minishell.h"
#include "expansion.h"
#include "redirection.h"

enum e_status	filename_expand(t_word *word, char **result, t_globals *g)
{
	enum e_status	status;
	t_word			*word_cpy;
	t_word_list		*original_list;
	t_word_list		*expanded_list;

	if (result)
		*result = NULL;
	if (!word || !result || !g)
		return (E_VALUE);
	status = SUCCESS;
	word_cpy = copy_word(word);
	if (!word_cpy)
		return (E_MALLOC);
	original_list = make_word_list(word_cpy, NULL);
	if (!original_list)
		return (destroy_word(word_cpy), E_MALLOC);
	expanded_list = expand_word_list(original_list, &status, g);
	if (status == E_MALLOC || !expanded_list || expanded_list->next)
		return (destroy_word_list(expanded_list), status);
	*result = string_list(expanded_list);
	destroy_word_list(expanded_list);
	if (!*result)
		return (E_MALLOC);
	return (SUCCESS);
}
	// if (g->posix)
	// 	word->flags |= W_NOSPLIT;

/* Unlike Bash: Not enough space for lenp. */
enum e_status	heredoc_expand(t_word *content, char **result, t_globals *g)
{
	enum e_status	status;

	if (!content || !result)
		return (E_VALUE);
	*result = content->word;
	if (!content->word || !content->word[0])
		return (SUCCESS);
	*result = NULL;
	status = SUCCESS;
	*result = expand_string_to_string(content->word, Q_HERE_DOCUMENT, \
				&status, g);
	return (status);
}
