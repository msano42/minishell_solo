#ifndef REDIRECTION_INTERNAL_H
# define REDIRECTION_INTERNAL_H

# include "command.h"
# include "variables.h"

/* backup_fd.c */
int				backup_fd(int io_fd, int file_fd, t_globals *g);

/* do_xxx_redirection.c */
int				do_io_redirection(t_redirect *redirect, int flags, \
				char **fnp, t_globals *g);
int				do_heredoc_redirection(t_redirect *redirect, int flags, \
				t_globals *g);
int				do_dup_redirection(t_redirect *redirect);
int				do_close_redirection(t_redirect *redirect, t_globals *g);

/* heredoc_to_fd.c */
int				heredoc_to_fd(t_word *content, enum e_status *status, \
				t_globals *g);

/* is_xxx_redirection.c */
bool			is_io_redirection(enum e_redir_type type);
bool			is_dup_redirection(enum e_redir_type type);
bool			is_expandable_filename_redirection(t_redirect *redirect);

/* redirection_expand.c */
enum e_status	filename_expand(t_word *word, char **result, t_globals *g);
enum e_status	heredoc_expand(t_word *content, char **result, t_globals *g);

#endif /* !REDIRECTION_INTERNAL_H */
