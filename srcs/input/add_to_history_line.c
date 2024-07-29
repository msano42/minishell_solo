#include <stddef.h>

#include "parser.h"
#include "general.h"
#include "wrappers.h" // memory

/* FIX THIS */
void	add_to_history_line(char *line, t_input *input)
{
	char		sep;
	size_t		history_line_len;
	char		*old_history_line;

	if (!input->history_line)
	{
		input->history_line = ft_strdup(line);
		if (!input->history_line)
			input->status = E_MALLOC;
		return ;
	}
	sep = '\n';
	history_line_len = ft_strlen(input->history_line);
	if (!input->quote && history_line_len >= 2 && \
		input->history_line[history_line_len - 1] == '\\' && \
		input->history_line[history_line_len - 2] != '\\')
	{
		sep = '\0';
		input->history_line[history_line_len - 1] = '\0';
	}
	old_history_line = input->history_line;
	input->history_line = join_with_separator(input->history_line, line, sep);
	if (!input->history_line)
		input->status = E_MALLOC;
	free(old_history_line);
}
