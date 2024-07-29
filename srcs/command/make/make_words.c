#include <stddef.h>

#include "command.h"
#include "general.h"
#include "wrappers.h" // memory

static t_word	*make_word_flags(t_word *word, const char *str);

t_word	*alloc_word(void)
{
	t_word	*word;

	word = xmalloc(sizeof(t_word));
	if (!word)
		return (NULL);
	word->word = NULL;
	word->flags = 0;
	return (word);
}

t_word	*make_bare_word(const char *str)
{
	t_word	*word;

	word = alloc_word();
	if (!word)
		return (NULL);
	if (str)
	{
		word->word = ft_strdup(str);
		if (!word->word)
		{
			free(word);
			return (NULL);
		}
	}
	return (word);
}

t_word	*make_word(const char *str)
{
	t_word	*word;

	word = make_bare_word(str);
	if (!word)
		return (NULL);
	return (make_word_flags(word, str));
}

/*
	If LIST is NULL, the new node becomes the only element in LIST.

	LIST is later reversed during parsing. Adding to the head removes
	the computational overhead incurred by traversal during insertion.
*/
t_word_list	*make_word_list(t_word *word, t_word_list *list)
{
	t_word_list	*new_head;

	new_head = xmalloc(sizeof(t_word_list));
	if (!new_head)
		return (NULL);
	new_head->word = word;
	new_head->next = list;
	return (new_head);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

/* Do we really need the return value?? */
static t_word	*make_word_flags(t_word *word, const char *str)
{
	if (!word || !str)
		return (word);
	while (*str)
	{
		if (*str == '$')
			word->flags |= W_HASDOLLAR;
		else if (*str == '\'' || *str == '"')
			word->flags |= W_QUOTED;
		str++;
	}
	return (word);
}

/* **************************************************************** */
/*	                 Static Functions Documentation	            	*/
/* **************************************************************** */

/**
	@fn static t_word	*make_word_flags(t_word *word, const char *str)

	@brief Sets flags of WORD according to STR.

	@param word Pointer to the t_word struct to set flags in.
	@param str String to set flags for.

	@return WORD

	@note The flags are not set in the same way as in the tokenizer.
	(ELABORATE)

	@diffbash 
		- Does not handle multibyte characters.
		- Does not consider back quotes '`' for W_QUOTED.

	@showrefs
	@showrefby
*/
