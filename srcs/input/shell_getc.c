#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "general.h"
#include "wrappers.h" // memory

static enum e_status	get_input_line(t_input *input);
static enum e_status	fill_input_line(t_input *input);
static enum e_status	resize_input_line_if_needed(t_input *input);
static enum e_status	handle_backslash_newline(t_input *input);

/* added || !input->globals->interactive */
int	shell_getc(bool remove_backslash_newline, t_input *input)
{
	char	c;

	if (input->input_lookahead_c)
	{
		c = input->input_lookahead_c;
		input->input_lookahead_c = 0;
		return (c);
	}
	if (!input->input_line || !input->input_line[input->input_index])
	{
		if (get_input_line(input) != SUCCESS)
			return (EOF);
	}
	if (remove_backslash_newline && handle_backslash_newline(input) != SUCCESS)
		return (EOF);
	c = input->input_line[input->input_index];
	if (c != '\0')
		input->input_index++;
	if (c == '\0' && input->input_line_terminator == EOF)
	{
		if (input->input_index == 0 || !input->globals->interactive)
			return (EOF);
		return ('\n');
	}
	return (c);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static enum e_status	get_input_line(t_input *input)
{
	clear_input_line(input);
	fill_input_line(input);
	if (input->status != SUCCESS)
		return (input->status);
	if (input->globals->interactive)
	{
		add_to_history_line(input->input_line, input);
		if (input->status != SUCCESS)
			return (input->status);
	}
	if (input->input_line_terminator != EOF)
	{
		input->input_line[input->input_len] = '\n';
		input->input_line[input->input_len + 1] = '\0';
	}
	return (SUCCESS);
}

/*  // added || !input->globals->interactive */
static enum e_status	fill_input_line(t_input *input)
{
	char	c;

	while (1)
	{
		c = input->getter(input);
		if (input->status != SUCCESS)
			return (input->status);
		if (c == '\0')
			continue ;
		resize_input_line_if_needed(input);
		if (input->status != SUCCESS)
			return (input->status);
		if (c == EOF || c == '\n')
		{
			if (c == EOF && (!input->input_len || !input->globals->interactive))
				input->input_line_terminator = EOF;
			input->input_line[input->input_len] = '\0';
			return (SUCCESS);
		}
		input->input_line[input->input_len++] = c;
	}
}

/* Treats SIZE_MAX overflow as malloc error. */
static enum e_status	resize_input_line_if_needed(t_input *input)
{
	if ((SIZE_MAX - 2 > input->input_len) \
		&& input->input_size > input->input_len + 2)
		return (SUCCESS);
	input->input_line = expand_buffer(input->input_line, input->input_size, \
						256);
	if (!input->input_line)
	{
		clear_input_line(input);
		input->status = E_MALLOC;
		return (E_MALLOC);
	}
	input->input_size += 256;
	return (SUCCESS);
}

/* Replaced with SIZE_MAX oveflow handling in expand_buffer() */
// if (input->input_size > (SIZE_MAX - 256))
// {
// 	clear_input_line(input);
// 	dprintf(2, "minishell: shell_getc: input line is too long: line aborted\n");
// 	input->status = E_OVERFLOW;
// 	return (E_OVERFLOW);
// }

static enum e_status	handle_backslash_newline(t_input *input)
{
	const char	*old_prompt = input->rl_current_prompt;

	if (!is_backslash_newline(&input->input_line[input->input_index]))
		return (SUCCESS);
	input->rl_current_prompt = input->rl_secondary_prompt;
	while (1)
	{
		get_input_line(input);
		if (input->status != SUCCESS)
			break ;
		if (!is_backslash_newline(input->input_line))
			break ;
	}
	input->rl_current_prompt = old_prompt;
	return (input->status);
}
