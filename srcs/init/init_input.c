#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>

#include "parser.h"
#include "general.h"
#include "variables.h"

#define HEART "\xE2\001\x9D\xA4\002 "
#define HEART_ARROW "\xE2\001\x9D\xA5\002 "

#define MINISHELL "minishell "

#define COLOR_RESET "\001\x1b[0m\002"
#define BOLD_PINK "\001\x1b[1;38;5;211m\002"

#define DEFAULT_FILENAME_QUOTE_CHARS " \t\n\\\"'@<>=;|&()#$`?*[!:{~"

static bool	is_interactive_shell(void);
static int	dummy_event_hook(void);
static void	init_readline(void);

void	init_input(t_input *input, t_globals *g)
{
	ft_memset(input, 0, sizeof(t_input));
	input->globals = g;
	g->interactive = is_interactive_shell();
	if (g->interactive)
	{
		get_tty_state(g);
		init_readline();
		input->getter = readline_getc;
		input->ungetter = readline_ungetc;
		input->rl_primary_prompt = HEART BOLD_PINK MINISHELL COLOR_RESET HEART;
		input->rl_secondary_prompt = HEART_ARROW;
		input->rl_current_prompt = input->rl_primary_prompt;
	}
	else
	{
		input->getter = stream_getc;
		input->ungetter = stream_ungetc;
	}
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static bool	is_interactive_shell(void)
{
	return (isatty(STDIN_FILENO) && isatty(STDERR_FILENO));
}

static void	init_readline(void)
{
	extern int	_rl_echo_control_chars;

	rl_instream = stdin;
	rl_outstream = stderr;
	_rl_echo_control_chars = 0;
	rl_catch_signals = 1;
	rl_event_hook = dummy_event_hook;
	rl_completer_quote_characters = "'\"";
	rl_filename_quote_characters = DEFAULT_FILENAME_QUOTE_CHARS;
}

static int	dummy_event_hook(void)
{
	return (0);
}
