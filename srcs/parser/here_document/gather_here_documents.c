#include <stdbool.h>
#include <stddef.h>

#include "minishell.h"
#include "parser.h"
#include "expansion.h"
#include "general.h"
#include "command.h"
#include "input.h"
#include "wrappers.h" // memory

#define HEREDOC_EOF_ERR_FORMAT "minishell: warning: here-document delimited by \
end-of-file (wanted `%s')\n"

static void	make_here_document(t_redirect *redir, t_input *input);
static char	*read_here_document(const char *delim, bool delim_is_quoted, \
			t_input *input);
static char	*read_heredoc_line(bool remove_quoted_newline, t_input *input);
static bool	handle_backslash_in_heredoc(t_input *input);

/* TODO: Make this file less fucking disgusting.  */

void	gather_here_documents(t_parser *parser)
{
	int	cnt;

	cnt = 0;
	while (cnt < parser->heredoc_count)
	{
		make_here_document(parser->heredoc_stack[cnt], parser->input);
		if (parser->input->status != SUCCESS)
		{
			parser->status = parser->input->status;
			return ;
		}
		parser->heredoc_stack[cnt] = NULL;
		cnt++;
	}
	parser->heredoc_count = 0;
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static void	make_here_document(t_redirect *redir, t_input *input)
{
	char		*document;
	t_word		word;
	const bool	delim_is_quoted = is_quoted_word(redir->redirectee.filename);

	word.flags = 0;
	word.word = NULL;
	input->word = &word;
	redir->here_doc_eof = redir->redirectee.filename->word;
	redir->redirectee.filename->word = NULL;
	if (delim_is_quoted)
		quote_removal_in_place(redir->here_doc_eof);
	input->rl_current_prompt = input->rl_secondary_prompt;
	document = read_here_document(redir->here_doc_eof, delim_is_quoted, input);
	input->rl_current_prompt = input->rl_primary_prompt;
	free(input->word->word);
	input->word = NULL;
	if (input->status == SUCCESS && !document)
		input->status = E_MALLOC;
	if (input->status != SUCCESS)
	{
		free(document);
		document = NULL;
	}
	redir->redirectee.filename->word = document;
}

static char	*read_here_document(const char *delim, bool delim_is_quoted, \
			t_input *input)
{
	char			*line;
	char			*document;
	char			*old_document;
	const size_t	delim_len = ft_strlen(delim);

	document = NULL;
	while (1)
	{
		line = read_heredoc_line(!delim_is_quoted, input);
		if (input->status != SUCCESS || !line || \
			(!ft_strncmp(line, delim, delim_len) && line[delim_len] == '\n'))
			break ;
		old_document = document;
		document = ft_strjoin(document, line);
		free(old_document);
		free(line);
		input->word->word = NULL;
		if (!document)
			return (NULL);
	}
	if (!line && input->status == SUCCESS)
		ft_dprintf(STDERR_FILENO, HEREDOC_EOF_ERR_FORMAT, delim);
	if (!document && input->status == SUCCESS)
		document = ft_strdup("");
	return (document);
}

static char	*read_heredoc_line(bool remove_backslash_newline, t_input *input)
{
	char	c;
	bool	pass_next_character;

	c = EOF;
	pass_next_character = false;
	while (c != '\n' && input->status == SUCCESS)
	{
		c = input->getter(input);
		if (input->status != SUCCESS || (c == EOF && !input->word->word))
			break ;
		else if (c == EOF)
			c = '\n';
		if (pass_next_character)
		{
			add_char_to_input_word(c, IGN_DOLLAR, IGN_ESC, input);
			pass_next_character = false;
		}
		else if (c == '\\' && remove_backslash_newline)
			pass_next_character = handle_backslash_in_heredoc(input);
		else
			add_char_to_input_word(c, IGN_DOLLAR, \
									remove_backslash_newline, input);
	}
	return (input->word->word);
}

static bool	handle_backslash_in_heredoc(t_input *input)
{
	int	next_char;

	next_char = input->getter(input);
	if (input->status != SUCCESS)
		return (false);
	if (next_char == '\n')
		return (false);
	else
	{
		input->ungetter(input);
		add_char_to_input_word('\\', IGN_DOLLAR, IGN_ESC, input);
		return (true);
	}
}
