#ifndef MINISHELL_H
# define MINISHELL_H

/* **************************************************************** */
/*	                          Defines	                            */
/* **************************************************************** */

# define CTLESC '\001'
# define CTLNUL '\177'

# define EXECUTION_SUCCESS			0
# define EXECUTION_FAILURE			1
# define EX_BADUSAGE				2
# define EXECUTION_MALLOC_FAILURE 	3
# define EX_USAGE					258

# define EX_NOEXEC			126

enum	e_status
{
	SUCCESS = 0,
	ERROR = 1,
	E_BADUSAGE = 2,
	E_MALLOC = 3,
	E_SYNTAX,
	E_WRITE,
	E_GETCWD,
	E_CHDIR,
	E_DUP,
	E_READ,
	E_OPEN,
	E_VALUE,
	E_CANON,
	E_EOF,
	E_UNCLOSED_QUOTE,
	E_OVERFLOW,
	E_INTERRUPTED,
	E_MAX_HEREDOC
};

#endif /* !MINISHELL_H */
