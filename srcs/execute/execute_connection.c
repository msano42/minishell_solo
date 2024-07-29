#include <stdbool.h>

#include "parser.h"
#include "execute.h"
#include "execute_internal.h"

static bool	should_execute_second_command(int first_result, int connector);

int	execute_connection(t_command *command, t_pipes pipes, \
	char *fds_to_close, t_globals *g)
{
	int			result;
	int			connector;
	t_command	*first;
	t_command	*second;

	connector = command->u_value.connection->connector;
	if (connector == '|')
		return (execute_pipeline(command, pipes, fds_to_close, g));
	first = command->u_value.connection->left;
	second = command->u_value.connection->right;
	result = execute_command(first, g);
	if (should_execute_second_command(result, connector))
		result = execute_command_internal(second, pipes, fds_to_close, g);
	return (result);
}

static bool	should_execute_second_command(int first_result, int connector)
{
	if (connector == ';')
		return (true);
	if (connector == TOK_AND_AND)
		return (first_result == EXECUTION_SUCCESS);
	if (connector == TOK_OR_OR)
		return (first_result != EXECUTION_SUCCESS);
	return (false);
}
