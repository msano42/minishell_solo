#include "command.h"
#include "parser.h"

static void	print_redirection_list(t_redirect *list);
void	print_word_list(t_word_list *list);

void print_ast(t_command *command, int level)
{
	if (!command)
		return ;
	if (command->type == CM_SUBSHELL)
	{
		printf("%*s%s\n", level * 6, "", "(");
		print_ast(command->u_value.subshell->command, level + 1);
		printf("%*s%s", level * 6, "", ")");
		print_redirection_list(command->redirects);
		printf("\n");
	}
	if (command->type == CM_CONNECTION)
	{
		print_ast(command->u_value.connection->right, level + 1);
		switch (command->u_value.connection->connector)
		{
			case TOK_AND_AND:
				printf("%*s%s\n", level * 6, "", "&&");
				break ;
			case TOK_OR_OR:
				printf("%*s%s\n", level * 6, "", "||");
				break ;
			case TOK_SEMICOLON:
				printf("%*s%s\n", level * 6, "", ";");
				break ;
			case TOK_PIPE:
				printf("%*s%s\n", level * 6, "", "|");
				break ;
		}
		print_ast(command->u_value.connection->left, level + 1);
	}
	if (command->type == CM_SIMPLE)
	{
		printf("%*s", level * 6, "");
		print_word_list(command->u_value.simple->words);
		print_redirection_list(command->u_value.simple->redirects);
		printf("\n");
		return ;
	}
}

void	print_word_list(t_word_list *list)
{
	while (list)
	{
		if (list->word)
			printf("%s ", list->word->word);
		list = list->next;
	}
}

static void	print_redirection_list(t_redirect *list)
{
	char	*redir_token;

	while (list)
	{
		switch (list->instruction)
		{
			case R_OUTPUT_DIRECTION:
				redir_token = ">";
				break ;
			case R_INPUT_DIRECTION:
				redir_token = "<";
				break ;
			case R_APPENDING_TO:
				redir_token = ">>";
				break ;
			case R_READING_UNTIL:
				redir_token = "<<";
				break ;
			default:
				redir_token = "?";
		}
		printf("%s %s ", redir_token, list->redirectee.filename->word);
		list = list->next;
	}
}
