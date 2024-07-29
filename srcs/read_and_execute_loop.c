#include "minishell.h"
#include "execute.h"
#include "general.h"
#include "parser.h"
#include "command.h"
#include "input.h"
#include "sig.h"
#include "wrappers.h" // memory

#define EXECUTE		0
#define CONTINUE	1
#define BREAK		2

void			simple_exec(t_command *command, t_globals *g);

void			init_parser(t_parser *parser, t_input *input, \
				t_globals *globals);
void			free_input(t_input *input);
void			free_parser(t_parser *parser);
void			reset_parser(t_parser *parser);

static int		interpret_parser_status(t_parser *parser);
static void		print_exit_if_interactive(t_globals *g);

int	read_and_execute_loop(t_input *input, t_globals *g)
{
	int			status;
	t_parser	parser;

	init_parser(&parser, input, g);
	while (!parser.input->eof_reached && !g->really_exit)
	{
		reset_parser(&parser);
		init_prompt_sighandlers(g->interactive);
		parse_command(&parser);
		status = interpret_parser_status(&parser);
		if (status == CONTINUE)
			continue ;
		if (status == BREAK)
			break ;
		// print_ast(parser.command, 0);
		// execute_command(parser.command, g);
		simple_exec(parser.command, g);
		destroy_command(parser.command);
		parser.command = NULL;
	}
	free_parser(&parser);
	return (g->last_command_exit_value);
}

void	init_parser(t_parser *parser, t_input *input, t_globals *globals)
{
	parser->input = input;
	parser->globals = globals;
	reset_parser(parser);
}

static void	print_exit_if_interactive(t_globals *g)
{
	if (g->interactive)
		ft_putstr_fd("exit\n", 2);
}

static int	interpret_parser_status(t_parser *parser)
{
	if (parser->status == E_INTERRUPTED)
		return (CONTINUE);
	if (parser->status == E_MALLOC || parser->status == E_MAX_HEREDOC)
		return (BREAK);
	if (parser->status == E_SYNTAX || parser->status == E_UNCLOSED_QUOTE)
	{
		if (!parser->globals->interactive)
			return (BREAK);
		if (!parser->input->eof_reached)
			return (CONTINUE);
		print_exit_if_interactive(parser->globals);
		return (BREAK);
	}
	if (parser->status == SUCCESS && !parser->command \
		&& parser->input->eof_reached)
	{
		print_exit_if_interactive(parser->globals);
		return (BREAK);
	}
	return (EXECUTE);
}

void	reset_input(t_input *input)
{
	input->status = SUCCESS;
	input->eof_reached = false;
	input->quote = 0;
	input->word = NULL;
	input->history_line = NULL;
	input->rl_current_prompt = input->rl_primary_prompt;
	clear_input_line(input);
}

void	reset_parser(t_parser *parser)
{
	parser->status = SUCCESS;
	parser->command = NULL;
	parser->element.word = NULL;
	parser->element.redirect = NULL;
	parser->current_token = TOK_UNDEFINED;
	parser->lookahead_token = TOK_UNDEFINED;
	parser->current_node_addr = &(parser->command);
	parser->current_root_addr = &(parser->command);
	parser->subshell_stack = NULL;
	parser->heredoc_count = 0;
	ft_memset(parser->heredoc_stack, 0, sizeof(parser->heredoc_stack));
	reset_input(parser->input);
}

void	free_input(t_input *input)
{
	destroy_word(input->word);
	input->word = NULL;
	free(input->history_line);
	input->history_line = NULL;
	clear_input_line(input);
	free(input->rl_line);
	input->rl_line = NULL;
}

void	free_parser(t_parser *parser)
{
	free_input(parser->input);
	destroy_command(parser->command);
	parser->command = NULL;
	destroy_word(parser->element.word);
	parser->element.word = NULL;
	destroy_redirects(parser->element.redirect);
	parser->element.redirect = NULL;
	free_subshell_stack(&parser->subshell_stack);
}
