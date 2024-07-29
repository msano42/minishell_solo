#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "sig.h"
#include "parser.h"
#include "general.h"
#include "minishell.h"
#include "wrappers.h" // memory

static void	readline_add_newline(t_input *input);

int	readline_getc(t_input *input)
{
	if (input->rl_line && !input->rl_line[input->rl_index])
	{
		free(input->rl_line);
		input->rl_line = NULL;
		input->rl_index = 0;
	}
	if (!input->rl_line)
	{
		if (unset_nodelay_mode(STDIN_FILENO) < 0)
		{
			ft_dprintf(STDERR_FILENO, NODELAY_ERR_FMT, strerror(errno));
			return (EOF);
		}
		readline_add_newline(input);
	}
	if (!input->rl_line)
		return (EOF);
	return (input->rl_line[input->rl_index++]);
}

void	readline_ungetc(t_input *input)
{
	if (input->rl_index)
		input->rl_index--;
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static void	readline_add_newline(t_input *input)
{
	char			*line;
	size_t			line_len;

	line = readline(input->rl_current_prompt);
	if (check_prompt_signals(input->globals) == SIGINT)
		input->status = E_INTERRUPTED;
	if (line && input->status != E_INTERRUPTED)
	{
		line_len = ft_strlen(line);
		input->rl_line = xmalloc(line_len + 2);
		if (input->rl_line)
		{
			ft_memmove(input->rl_line, line, line_len);
			input->rl_line[line_len] = '\n';
			input->rl_line[line_len + 1] = '\0';
		}
		else
			input->status = E_MALLOC;
	}
	free(line);
}
