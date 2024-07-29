#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h> // strerror()

#include "parser.h"
#include "general.h"
#include "wrappers.h"

static int	reset_buf_return_eof(t_input *input);

int	stream_getc(t_input *input)
{
	if (input->buf_index == input->buf_used || input->buf_used == 0)
	{
		while (1)
		{
			errno = 0;
			input->buf_used = xread(STDIN_FILENO, input->buf, STREAM_BUF_SIZE);
			if (input->buf_used > 0)
				break ;
			else if (input->buf_used == 0)
				return (reset_buf_return_eof(input));
			else if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				if (unset_nodelay_mode(STDIN_FILENO) < 0)
				{
					ft_dprintf(STDERR_FILENO, NODELAY_ERR_FMT, strerror(errno));
					return (reset_buf_return_eof(input));
				}
				continue ;
			}
			else if (errno)
				return (reset_buf_return_eof(input));
		}
		input->buf_index = 0;
	}
	return ((unsigned char)input->buf[input->buf_index++]);
}

void	stream_ungetc(t_input *input)
{
	if (input->buf_index)
		input->buf_index--;
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static int	reset_buf_return_eof(t_input *input)
{
	input->buf_index = 0;
	input->buf_used = 0;
	return (EOF);
}
