#ifndef COMMAND_H
# define COMMAND_H

# include <stdint.h>

# include "minishell.h"

/* **************************************************************** */
/*	                          Defines	                            */
/* **************************************************************** */

/* Values for 'flags' of a t_word struct. */
# define W_HASDOLLAR		0x1
# define W_QUOTED			0x2
# define W_NOGLOB			0x4
# define W_NOSPLIT			0x8
# define W_HASQUOTEDNULL	0x10
# define W_SAWQUOTEDNULL	0x20

/* Values for 'flags' of a t_command, t_simple_command or 
t_subshell_command struct. */
# define CMD_WANT_SUBSHELL	0x01
# define CMD_NO_FORK		0x02

/* **************************************************************** */
/*	                           Structs	                            */
/* **************************************************************** */

/* ========== words =========== */

typedef struct s_word
{
	char		*word;
	uint8_t		flags;
}	t_word;

typedef struct s_word_list
{
	t_word				*word;
	struct s_word_list	*next;
}	t_word_list;

/* ========== redirections =========== */

typedef union u_redir_target
{
	int		fd;
	t_word	*filename;
}	t_redir_target;

enum e_redir_type
{
	R_OUTPUT_DIRECTION,
	R_APPENDING_TO,
	R_INPUT_DIRECTION,
	R_READING_UNTIL,
	R_DUPLICATING_INPUT,
	R_DUPLICATING_OUTPUT,
	R_CLOSE_THIS
};

typedef struct s_redirect
{
	struct s_redirect	*next;
	int					open_flags;
	enum e_redir_type	instruction;
	t_redir_target		redirector;
	t_redir_target		redirectee;
	char				*here_doc_eof;
}	t_redirect;

/* ========== commands =========== */

typedef struct s_element
{
	t_word		*word;
	t_redirect	*redirect;
}	t_element;

enum e_command_type
{
	CM_SIMPLE,
	CM_CONNECTION,
	CM_SUBSHELL
};

typedef struct s_command
{
	enum e_command_type	type;
	uint8_t				flags;
	t_redirect			*redirects;
	union
	{
		struct s_connection			*connection;
		struct s_simple_command		*simple;
		struct s_subshell_command	*subshell;
	} u_value;
}	t_command;

typedef struct s_connection
{
	t_command	*left;
	t_command	*right;
	int			connector;
}	t_connection;

typedef struct s_simple_command
{
	uint8_t		flags;
	t_word_list	*words;
	t_redirect	*redirects;
}	t_simple_command;

typedef struct s_subshell_command
{
	uint8_t		flags;
	t_command	*command;
}	t_subshell_command;

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

/* ========== make =========== */

/* make_commands.c */
t_command		*make_simple_command(void);
t_command		*make_subshell_command(void);
t_command		*make_connection_command(int connector);

/* make_redirection.c */
t_redirect		*make_redirection(t_redir_target source, \
				enum e_redir_type instruction, \
				t_redir_target dest);

/* make_words.c */
t_word			*alloc_word(void);
t_word			*make_bare_word(const char *str);
t_word			*make_word(const char *str);
t_word_list		*make_word_list(t_word *word, t_word_list *head);
t_word_list		*make_word_list_from_str(const char *str);
void			word_list_append(t_word_list **list, t_word_list *new);

/* add_to_xxx_list.c */
enum e_status	add_to_word_list(t_word *word, t_word_list **list);
void			add_to_redirect_list(t_redirect *redirect, \
				t_redirect **list);

/* ========== copy =========== */

t_word			*copy_word(t_word *word);

/* ========== destroy =========== */

/* destroy_command.c */
void			destroy_command(t_command *command);

/* destroy_words.c */
void			destroy_word(t_word *word);
void			destroy_word_list(t_word_list *list);

/* destroy_redirects.c */
void			destroy_redirects(t_redirect *list);

#endif /* !COMMAND_H */
