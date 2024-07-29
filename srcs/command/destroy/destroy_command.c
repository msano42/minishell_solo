#include "command.h"
#include "wrappers.h" // memory

void	destroy_command(t_command *command)
{
	if (!command)
		return ;
	if (command->type == CM_SIMPLE && command->u_value.simple)
	{
		destroy_word_list(command->u_value.simple->words);
		destroy_redirects(command->u_value.simple->redirects);
		free(command->u_value.simple);
	}
	if (command->type == CM_SUBSHELL && command->u_value.subshell)
	{
		destroy_command(command->u_value.subshell->command);
		free(command->u_value.subshell);
	}
	if (command->type == CM_CONNECTION && command->u_value.connection)
	{
		destroy_command(command->u_value.connection->left);
		destroy_command(command->u_value.connection->right);
		free(command->u_value.connection);
	}
	destroy_redirects(command->redirects);
	free(command);
}
