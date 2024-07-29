#include <stddef.h>

#include "minishell.h"
#include "parser.h"
#include "command.h"

static enum e_status	add_element_to_command(t_command *command, \
						t_element element);
static t_word_list		**get_command_word_list_addr(t_command *command);
static t_redirect		**get_command_redir_list_addr(t_command *command);

void	parse_simple_element(t_parser *parser)
{
	t_command	*command;

	command = *(parser->current_node_addr);
	if (command && command->type == CM_SUBSHELL && parser->element.word)
	{
		parser->status = E_SYNTAX;
		return ;
	}
	if (!command)
		command = make_simple_command();
	if (!command)
	{
		parser->status = E_MALLOC;
		return ;
	}
	*(parser->current_node_addr) = command;
	parser->status = add_element_to_command(command, parser->element);
	if (parser->status != SUCCESS)
		return ;
	parser->element.word = NULL;
	parser->element.redirect = NULL;
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static enum e_status	add_element_to_command(t_command *command, \
						t_element element)
{
	t_word_list	**word_list_addr;
	t_redirect	**redir_list_addr;

	if (!command)
		return (E_VALUE);
	if (element.word)
	{
		word_list_addr = get_command_word_list_addr(command);
		return (add_to_word_list(element.word, word_list_addr));
	}
	if (element.redirect)
	{
		redir_list_addr = get_command_redir_list_addr(command);
		add_to_redirect_list(element.redirect, redir_list_addr);
	}
	return (SUCCESS);
}

static t_word_list	**get_command_word_list_addr(t_command *command)
{
	if (!command || command->type != CM_SIMPLE || !command->u_value.simple)
		return (NULL);
	return (&(command->u_value.simple->words));
}

static t_redirect	**get_command_redir_list_addr(t_command *command)
{
	if (!command)
		return (NULL);
	if (command->type == CM_SUBSHELL)
		return (&(command->redirects));
	if (command->type == CM_SIMPLE && command->u_value.simple)
		return (&(command->u_value.simple->redirects));
	return (NULL);
}
