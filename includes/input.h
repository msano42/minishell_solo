#ifndef INPUT_H
# define INPUT_H

# include <stddef.h>
# include <stdbool.h>
# include <sys/types.h>

# include "minishell.h"
# include "command.h"
# include "variables.h"

/* **************************************************************** */
/*	                          Defines	                            */
/* **************************************************************** */

/** Buffer size for read(2) calls in stream_getc() (non-interactive mode). */
# define STREAM_BUF_SIZE 1024

# define NODELAY_ERR_FMT "minishell: cannot reset nodelay mode for fd 0: %s\n"

/* Values for 'remove_backslash_newline' argument to shell_getc(). */
# define REMOVE_BACKSLASH_NEWLINE 1
# define KEEP_BACKSLASH_NEWLINE 0

/* Values for 'flag_dollar' argument to add_char_to_input_word(). */
# define FLAG_DOLLAR 1
# define IGN_DOLLAR 0

# define IGN_ESC 0
# define QUOTE_ESC 1

/* **************************************************************** */
/*	                           Structs	                            */
/* **************************************************************** */

typedef struct s_input	t_input;

typedef int				t_getc_func(t_input *);
typedef void			t_ungetc_func(t_input *);

struct s_input
{
	enum e_status	status;

	t_globals		*globals;

	bool			eof_reached;

	int				quote;
	t_word			*word;
	char			*history_line;

	t_getc_func		*getter;
	t_ungetc_func	*ungetter;

	char			*input_line;
	size_t			input_len;
	size_t			input_size;
	size_t			input_index;
	char			input_line_terminator;
	char			input_lookahead_c;

	char			*rl_line;
	size_t			rl_index;
	const char		*rl_current_prompt;
	const char		*rl_primary_prompt;
	const char		*rl_secondary_prompt;

	char			buf[STREAM_BUF_SIZE];
	ssize_t			buf_used;
	ssize_t			buf_index;
};

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

/* shell_getc.c */
int				shell_getc(bool remove_backslash_newline, t_input *input);
/* shell_unget.c */
void			shell_ungetc(int c, t_input *input);

/* readline_get_unget.c */
int				readline_getc(t_input *input);
void			readline_ungetc(t_input *input);

/* stream_get_unget.c */
int				stream_getc(t_input *input);
void			stream_ungetc(t_input *input);

/* add_to_history_line.c */
void			add_to_history_line(char *line, t_input *input);

/* clear_input_line.c */
void			clear_input_line(t_input *input);

/* tty_state.c */
void			get_tty_state(t_globals *g);
void			set_tty_state(t_globals *g);

/* unset_nodelay_mode.c */
int				unset_nodelay_mode(int fd);

/* add_char_to_input_word.c */
enum e_status	add_char_to_input_word(char c, bool flag_dollar, \
				bool quote_escape, t_input *input);

#endif /* !INPUT_H */
