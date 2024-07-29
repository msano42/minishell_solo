#include "minishell.h"
#include "parser.h"
#include "parser_internal.h"

void	parse_subshell_start(t_parser *parser)
{
	t_command	*subshell_command;
	t_command	*current_command;

	current_command = *(parser->current_node_addr);
	if (current_command)
	{
		parser->status = E_SYNTAX;
		return ;
	}
	subshell_command = make_subshell_command();
	if (!subshell_command)
	{
		parser->status = E_MALLOC;
		return ;
	}
	*(parser->current_node_addr) = subshell_command;
	push_subshell_stack(parser->current_root_addr, parser->current_node_addr, \
	&(parser->subshell_stack));
	if (!parser->subshell_stack)
	{
		parser->status = E_MALLOC;
		return ;
	}
	parser->current_node_addr = &(subshell_command->u_value.subshell->command);
	parser->current_root_addr = &(subshell_command->u_value.subshell->command);
}

void	parse_subshell_end(t_parser *parser)
{
	if (!parser->subshell_stack || !*(parser->current_node_addr))
	{
		parser->status = E_SYNTAX;
		return ;
	}
	parser->current_node_addr = parser->subshell_stack->start;
	parser->current_root_addr = parser->subshell_stack->root;
	pop_subshell_stack(&parser->subshell_stack);
}
