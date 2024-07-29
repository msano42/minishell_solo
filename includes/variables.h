#ifndef VARIABLES_H
# define VARIABLES_H

# include <stdbool.h>
# include <limits.h>
# include <termios.h>

# include "minishell.h"
# include "hashlib.h"
# include "command.h"
# include "jobs.h"

# define ATT_EXPORTED	0x01
# define ATT_INVISIBLE	0x02
# define ATT_IMPORTED	0x04

/* Flags for var_context->flags */
# define VC_HASLOCAL	0x01
# define VC_HASTMPVAR	0x02
# define VC_FUNCENV		0x04
# define VC_BLTNENV		0x08
# define VC_TEMPENV		0x10

/* flags for find_variable_internal */
# define FV_FORCETEMPENV	0x01
# define FV_SKIPINVISIBLE	0x02
# define FV_NODYNAMIC		0x04

# define ASS_APPEND			0x01

typedef struct s_shell_var
{
	char	*name;
	char	*value;
	char	*exportstr;
	int		attributes;
	int		context;
}	t_shell_var;

typedef bool	t_var_selection_func(t_shell_var *var);

typedef struct s_var_context
{
	char					*name;
	int						scope;
	int						flags;
	struct s_var_context	*up;
	struct s_var_context	*down;
	t_hash_table			*table;
}	t_var_context;

/* Pass this to every function to replicate global variables in bash. */
typedef struct s_global_variables
{
	bool			posix;
	bool			interactive;

	int				last_command_exit_value;
	char			*current_working_directory;
	int				shell_level;
	bool			really_exit;

	bool			got_tty_state;
	struct termios	shell_tty_info;

	pid_t			last_made_pid;
	bool			already_making_children;
	t_pid_list		*pid_list;

	char			**export_env;
	int				export_env_size;
	int				export_env_index;
	bool			array_needs_making;

	t_var_context	*global_variables;
	t_var_context	*shell_variables;

	int				variable_context;

	t_hash_table	*temporary_env;
	t_hash_table	*hashed_filenames;
	t_hash_table	*last_table_searched;
	t_hash_table	*table_to_bind_to;

	t_shell_var		*ifs_var;
	char			*ifs_value;
	bool			ifs_is_set;
	bool			ifs_is_null;
	unsigned char	ifs_firstc;
	unsigned char	ifs_cmap[UCHAR_MAX + 1];

	bool			expanding_redir;
	bool			executing_builtin;
	int				subshell_environment;

	int				heredoc_errno;
	t_redirect		*redirection_undo_list;
}	t_globals;

typedef struct s_bind_args
{
	uint8_t			hflags;
	uint8_t			aflags;
	t_hash_table	*table;
}	t_bind_args;

void			destroy_var_context(t_var_context *context);
t_var_context	*new_var_context(const char *name, uint8_t flags, int scope);
enum e_status	adjust_shell_level(int change, t_globals *g);
char			*get_string_value(const char *name, t_globals *g);
t_shell_var		*bind_variable(const char *name, char *value, \
								uint8_t flags, t_globals *g);
t_shell_var		*find_variable(const char *name, t_globals *g);
t_shell_var		*hash_lookup(const char *name, t_hash_table *table, \
							t_globals *g);
bool			is_imported_var(t_shell_var *var);
bool			is_exported_var(t_shell_var *var);
bool			is_invisible_var(t_shell_var *var);
t_shell_var		*set_if_not(char *name, char *value, t_globals *g);
void			set_ifs(t_shell_var *ifs, t_globals *g);
enum e_status	create_variable_table(t_globals *g);
void			free_variable_hash_data(void *data);
void			destroy_variable(t_shell_var *var);
t_shell_var		*new_variable(const char *name, t_hash_table *table);
t_shell_var		*var_lookup(const char *name, t_var_context *context, \
							t_globals *g);
char			*make_variable_value(t_shell_var *var, char *value, \
				uint8_t flags);
t_shell_var		*bind_variable_internal(const char *name, char *value, \
									t_bind_args args, t_globals *g);
t_bind_args		gen_bind_args(t_hash_table *table, uint8_t hflags, \
				uint8_t aflags);
void			unbind_variable(const char *name, t_globals *g);
void			handle_if_special_variable_modified(const char *name, \
				t_globals *g);
void			**make_variable_array(t_var_selection_func *func, \
				t_hash_table *table);

/* TODO */
void			*sv_ifs(char *name, t_globals *g);

#endif /* !VARIABLES_H */
