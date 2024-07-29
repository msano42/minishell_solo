#include "parser.h"
#include "wrappers.h" // memory

void	clear_input_line(t_input *input)
{
	free(input->input_line);
	input->input_line = NULL;
	input->input_len = 0;
	input->input_size = 0;
	input->input_index = 0;
	input->input_line_terminator = '\0';
}
