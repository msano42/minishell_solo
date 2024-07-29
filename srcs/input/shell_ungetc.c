#include "parser.h"

void	shell_ungetc(int c, t_input *input)
{
	if (input->input_line && input->input_index)
		input->input_index--;
	else
		input->input_lookahead_c = c;
}
