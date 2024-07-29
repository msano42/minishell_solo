#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "minishell.h"
#include "general.h"
#include "command.h"
#include "parser.h"
#include "parser_internal.h"
#include "wrappers.h" // memory

static void	parser_finish(t_parser *parser);
static void	clean_up_parser(t_parser *parser);
static void	minishell_add_history(t_input *input);

void		make_here_document(t_redirect *redirect, t_input *input);

void	parse_command(t_parser *parser)
{
	while (parser->status == SUCCESS)
	{
		parser->current_token = get_token(parser);
		if (parser->status != SUCCESS)
			break ;
		if (is_end_of_command_token(parser->current_token))
			break ;
		if (is_redirection_token(parser->current_token))
			parse_redirection(parser);
		if (parser->element.word || parser->element.redirect)
			parse_simple_element(parser);
		else if (is_connection_token(parser->current_token))
			parse_connection(parser);
		else if (parser->current_token == TOK_L_PAREN)
			parse_subshell_start(parser);
		else if (parser->current_token == TOK_R_PAREN)
			parse_subshell_end(parser);
	}
	parser_finish(parser);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static void	parser_finish(t_parser *parser)
{
	handle_parser_error(parser);
	if (parser->status == SUCCESS)
		gather_here_documents(parser);
	minishell_add_history(parser->input);
	clean_up_parser(parser);
	if (parser->status != SUCCESS)
	{
		destroy_command(parser->command);
		parser->command = NULL;
	}
}

static void	minishell_add_history(t_input *input)
{
	if (input->history_line && input->history_line[0])
		add_history(input->history_line);
	free(input->history_line);
	input->history_line = NULL;
}

/*
	Clears everything except parser->status, parser->command
	and parser->input->eof_reached.

	NB: A lot of this is redundant but here for safety.
*/
static void	clean_up_parser(t_parser *parser)
{
	destroy_word(parser->element.word);
	destroy_redirects(parser->element.redirect);
	parser->element.word = NULL;
	parser->element.redirect = NULL;
	parser->lookahead_token = TOK_UNDEFINED;
	parser->current_token = TOK_UNDEFINED;
	parser->current_node_addr = &(parser->command);
	parser->current_root_addr = &(parser->command);
	free_subshell_stack(&parser->subshell_stack);
	parser->subshell_stack = NULL;
	parser->heredoc_count = 0;
	ft_memset(parser->heredoc_stack, 0, sizeof(parser->heredoc_stack));
	parser->input->status = SUCCESS;
	parser->input->quote = 0;
	destroy_word(parser->input->word);
	parser->input->word = NULL;
	free(parser->input->history_line);
	parser->input->history_line = NULL;
	clear_input_line(parser->input);
	parser->input->rl_current_prompt = parser->input->rl_primary_prompt;
}
