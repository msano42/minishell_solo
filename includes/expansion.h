#ifndef EXPANSION_H
# define EXPANSION_H

# include <stddef.h>

# include "minishell.h"
# include "variables.h"

# define Q_DOUBLE_QUOTES	0x01
# define Q_HERE_DOCUMENT	0x02

# define CTLNUL_STR "\177"

enum e_quoted_state
{
	UNQUOTED = 0,
	WHOLLY_QUOTED,
	PARTIALLY_QUOTED
};

typedef struct s_expansion
{
	enum e_status		status;

	bool				in_dquotes;
	bool				in_heredoc;

	size_t				idx;
	char				*expanded_str;

	enum e_quoted_state	quoted_state;
	bool				had_quoted_null;
	bool				has_quoted_ifs;
	bool				expanded_something;
	bool				ign_expanded_something;
}	t_expansion;

bool		is_invisible_var(t_shell_var *var);

/* add_xxx_to_expansion.c */
void		add_char_to_expansion(char c, t_expansion *vars);
void		add_string_to_expansion(const char *str, t_expansion *vars);
void		add_quoted_string_to_expansion(const char *str, t_expansion *vars);

/* case_backslash.c */
void		case_backslash(const char *str, t_expansion *vars, t_globals *g);

/* case_ctlesc.c */
void		case_ctlesc(const char *str, t_expansion *vars);

/* case_default.c */
void		case_default(const char *str, uint8_t word_flags, \
						t_expansion *exp, t_globals *g);

/* case_dollar.c */
void		case_dollar(const char *str, t_expansion *exp, t_globals *g);

/* case_double_quote.c*/
void		case_double_quote(const char *str, uint8_t word_flags, \
			t_expansion *exp, t_globals *g);

/* case_single_quote.c */
void		case_single_quote(const char *str, uint8_t word_flags, \
			t_expansion *exp, const char *ifs_value);

/* finish_expansion.c */
t_word_list	*finish_expansion(uint8_t original_flags, t_expansion *exp, \
			t_globals *g);

/* in_dquotes_or_heredoc.c */
bool		in_dquotes_or_heredoc(t_expansion *vars);

bool		is_ifs(char c, t_globals *g);

t_word_list	*expand_word(t_word *word, t_expansion *exp, t_globals *g);
t_word_list	*expand_word_list(t_word_list *list, enum e_status *status, \
			t_globals *g);

char		*expand_string_to_string(const char *str, int quoted, \
			enum e_status *status, t_globals *g);

void		init_expansion_struct(t_expansion *exp, uint8_t quoted, \
			bool ign_expanded_something);

char		*quote_escapes(const char *str, const char *ifs_value);
char		*dequote_escapes_in_place(char *str, const char *ifs_value);
char		*quote_string(const char *str);
void		remove_quoted_ifs_in_place(char *str, t_globals *g);
char		*remove_quoted_nulls_in_place(char *str);
void		dequote_list(t_word_list *list);
void		dequote_string_in_place(char *str);
void		list_remove_quoted_nulls(t_word_list *list);

char		*string_list(t_word_list *list);

void		quote_removal_in_place(char *str);
t_word_list	*list_string(const char *str, const char *separators, \
			uint8_t quoted, enum e_status *status);

bool		is_quoted_null(const char *str);

char		*extract_single_quoted_section(const char *str, size_t *idx_ptr);
char		*extract_double_quoted_section(const char *str, size_t *idx_ptr);

#endif /* !EXPANSION_H */
