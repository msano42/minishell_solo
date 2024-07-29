#include <stddef.h>
#include <stdbool.h>

#include "minishell.h"
#include "parser.h"
#include "parser_internal.h"
#include "command.h"

#define LEFT 1
#define RIGHT 0

static bool	should_ignore_semicolon(enum e_token token);
static void	insert_connection(t_command *connection, t_parser *parser);
static int	precedence_level(t_command *command);
static int	associativity(t_command *command);

void	parse_connection(t_parser *parser)
{
	t_command		*current_command;
	t_command		*connection;

	current_command = *(parser->current_node_addr);
	if (!current_command)
	{
		parser->status = E_SYNTAX;
		return ;
	}
	if (parser->current_token == TOK_SEMICOLON)
	{
		parser->lookahead_token = get_token(parser);
		if (parser->status != SUCCESS)
			return ;
		if (should_ignore_semicolon(parser->lookahead_token))
			return ;
	}
	connection = make_connection_command(parser->current_token);
	if (!connection)
	{
		parser->status = E_MALLOC;
		return ;
	}
	insert_connection(connection, parser);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static bool	should_ignore_semicolon(enum e_token token)
{
	return (is_end_of_command_token(token) || token == TOK_R_PAREN);
}

/* TODO: Save insertion points by precedence to minimize traversal time. */
static void	insert_connection(t_command *connection, t_parser *parser)
{
	t_command	*child_node;
	t_command	*parent_node;
	int			child_precedence;
	const int	connection_precedence = precedence_level(connection);
	const bool	is_left_asociative = (associativity(connection) == LEFT);

	parent_node = NULL;
	child_node = *(parser->current_root_addr);
	while (child_node)
	{
		child_precedence = precedence_level(child_node);
		if (connection_precedence < child_precedence)
			break ;
		if (is_left_asociative && connection_precedence == child_precedence)
			break ;
		parent_node = child_node;
		child_node = child_node->u_value.connection->right;
	}
	connection->u_value.connection->left = child_node;
	if (parent_node)
		parent_node->u_value.connection->right = connection;
	else
		*(parser->current_root_addr) = connection;
	parser->current_node_addr = &(connection->u_value.connection->right);
}

static int	precedence_level(t_command *command)
{
	int	connector;

	if (!command)
		return (-1);
	if (command->type == CM_CONNECTION)
	{
		connector = command->u_value.connection->connector;
		if (connector == TOK_SEMICOLON || connector == TOK_NEWLINE)
			return (1);
		if (connector == TOK_AND_AND || connector == TOK_OR_OR)
			return (2);
		if (connector == TOK_PIPE)
			return (3);
	}
	return (4);
}

static int	associativity(t_command *command)
{
	int	connector;

	if (!command)
		return (-1);
	if (command->type != CM_CONNECTION || !command->u_value.connection)
		return (-1);
	connector = command->u_value.connection->connector;
	if (connector == TOK_PIPE)
		return (RIGHT);
	return (LEFT);
}
