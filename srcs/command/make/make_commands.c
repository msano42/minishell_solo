#include <stddef.h>

#include "command.h"
#include "wrappers.h" // memory

static t_command	*make_command(enum e_command_type type, \
					void *value);

t_command	*make_simple_command(void)
{
	t_command			*command;
	t_simple_command	*simple;

	simple = malloc(sizeof(t_simple_command));
	if (!simple)
		return (NULL);
	simple->flags = 0;
	simple->words = NULL;
	simple->redirects = NULL;
	command = make_command(CM_SIMPLE, simple);
	if (!command)
		free(simple);
	return (command);
}

t_command	*make_connection_command(int connector)
{
	t_command		*command;
	t_connection	*connection;

	connection = malloc(sizeof(t_connection));
	if (!connection)
		return (NULL);
	connection->left = NULL;
	connection->right = NULL;
	connection->connector = connector;
	command = make_command(CM_CONNECTION, connection);
	if (!command)
		free(connection);
	return (command);
}

t_command	*make_subshell_command(void)
{
	t_command			*command;
	t_subshell_command	*subshell;

	subshell = malloc(sizeof(t_subshell_command));
	if (!subshell)
		return (NULL);
	subshell->flags = 0;
	subshell->command = NULL;
	command = make_command(CM_SUBSHELL, subshell);
	if (!command)
	{
		free(subshell);
		return (NULL);
	}
	command->flags = CMD_WANT_SUBSHELL;
	return (command);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static t_command	*make_command(enum e_command_type type, \
					void *value)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->type = type;
	command->flags = 0;
	command->redirects = NULL;
	command->u_value.simple = value;
	return (command);
}

/* **************************************************************** */
/*	                 Static Functions Documentation	            	*/
/* **************************************************************** */

/**
	@fn static t_command	*make_command(enum e_command_type type, \
	void *value)

	@brief Allocates a t_command, initializing the type to \ar type
	and the value to \ar value.

	All other fields are initialized to 0 or NULL.

	@param The Type of the command to create.
	@param value Pointer to a t_simple_command, t_connection or
	t_subshell_command.

	@return Pointer to allocated t_command or NULL if allocation fails.

	@showrefby
	@showrefs
*/