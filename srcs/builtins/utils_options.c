#include <unistd.h>
#include <stdbool.h>

#include "general.h"
#include "builtins.h"
#include "builtins_internal.h"

#include <string.h> // ft_strncmp()

#define INVALID_OPT_ERR_FORMAT "minishell: %s: -%c: invalid option\n"

static bool	is_option(const char *str);

/* TODO: convert to general getopt(). */
int	check_options(t_word_list **list, const char *name, const char *usage, \
const char *longdoc)
{
	t_word_list	*head;

	head = *list;
	if (!head || !head->word || !head->word->word)
		return (EXECUTION_SUCCESS);
	if (!is_option(head->word->word))
		return (EXECUTION_SUCCESS);
	else if (is_help_option(head->word->word))
	{
		print_usage(name, usage, longdoc);
		return (EX_BADUSAGE);
	}
	else if (is_end_of_options(head->word->word))
	{
		*list = head->next;
		return (EXECUTION_SUCCESS);
	}
	else
	{
		ft_dprintf(STDERR_FILENO, INVALID_OPT_ERR_FORMAT, name, \
					head->word->word[1]);
		print_usage(name, usage, NULL);
		return (EX_BADUSAGE);
	}
}

bool	is_help_option(const char *str)
{
	if (!str)
		return (false);
	return (!ft_strncmp(str, "--help", 7));
}

bool	is_end_of_options(const char *word)
{
	if (!word)
		return (false);
	return (word[0] == '-' && word[1] == '-' && word[2] == '\0');
}

void	print_usage(const char *name, const char *usage, const char *longdoc)
{
	ft_dprintf(STDERR_FILENO, "%s: usage: %s\n", name, usage);
	if (longdoc)
		ft_dprintf(STDERR_FILENO, "%s\n", longdoc);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static bool	is_option(const char *str)
{
	if (!str)
		return (false);
	return (str[0] == '-' && str[1] != '\0');
}
