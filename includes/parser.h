#ifndef PARSER_H
# define PARSER_H

# include <limits.h>
# include <stdbool.h>
# include <stdio.h>

# include "minishell.h"
# include "command.h"
# include "variables.h"
# include "input.h"

/* **************************************************************** */
/*	                          Defines	                            */
/* **************************************************************** */

/** Add this value to double tokens ( >>, <<, &&, || ) to avoid overlap. */
# define DOUBLE_TOKEN_OFFSET UCHAR_MAX

/** Maximum number of heredocuments allowed (HEREDOC_MAX is NOT allowed). */
# define HEREDOC_MAX 16

/* **************************************************************** */
/*	                           Structs	                            */
/* **************************************************************** */

enum e_token
{
	TOK_EOF = EOF,
	TOK_WORD = 0,
	TOK_NEWLINE = '\n',
	TOK_L_PAREN = '(',
	TOK_R_PAREN = ')',
	TOK_SEMICOLON = ';',
	TOK_LESS = '<',
	TOK_GREATER = '>',
	TOK_PIPE = '|',
	TOK_LESS_LESS = '<' + DOUBLE_TOKEN_OFFSET,
	TOK_GREATER_GREATER = '>' + DOUBLE_TOKEN_OFFSET,
	TOK_AND_AND = '&' + DOUBLE_TOKEN_OFFSET,
	TOK_OR_OR = '|' + DOUBLE_TOKEN_OFFSET,
	TOK_UNDEFINED
};

/*
	Token values:

	TOK_EOF				= -1
	TOK_WORD			= 0
	TOK_NEWLINE			= 10
	TOK_L_PAREN			= 40
	TOK_R_PAREN			= 41
	TOK_SEMICOLON		= 59
	TOK_LESS			= 60
	TOK_GREATER			= 62
	TOK_PIPE			= 124
	TOK_LESS_LESS		= 316
	TOK_GREATER_GREATER	= 318
	TOK_AND_AND			= 294
	TOK_OR_OR			= 380
*/

typedef struct s_subshell_stack
{
	t_command				**start;
	t_command				**root;
	struct s_subshell_stack	*next;
}	t_subshell_stack;

typedef struct s_parser
{
	enum e_status		status;

	t_input				*input;
	t_globals			*globals;

	t_command			*command;

	t_element			element;

	enum e_token		current_token;
	enum e_token		lookahead_token;

	t_command			**current_node_addr;
	t_command			**current_root_addr;

	t_subshell_stack	*subshell_stack;

	int					heredoc_count;
	t_redirect			*heredoc_stack[HEREDOC_MAX];
}	t_parser;

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

/* parse_command.c */
void	parse_command(t_parser *parser);
void	init_parser(t_parser *parser, t_input *input, t_globals *globals);

/* subshell_stack.c */
void	push_subshell_stack(t_command **root, t_command **start, \
		t_subshell_stack **stack);
void	pop_subshell_stack(t_subshell_stack **stack);
void	free_subshell_stack(t_subshell_stack **stack);

#endif /* !PARSER_H */
