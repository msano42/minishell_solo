#ifndef GENERAL_H
# define GENERAL_H

# include <stdint.h>
# include <stdbool.h>
# include <stddef.h>
# include <sys/types.h>

# include "minishell.h"
# include "variables.h"

# include <stdio.h>

#define ft_dprintf(fd, fmt, ...) \
    dprintf(fd, fmt, ##__VA_ARGS__)

/* **************************************************************** */
/*	                          Defines	                            */
/* **************************************************************** */

// # define MP_DOTILDE	0x01
// # define MP_DOCWD	0x02
# define MP_RMDOT	0x04
# define MP_IGNDOT	0x08

typedef int	t_comp_func(void *ptr_1, void *ptr_2);

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

/* is_legal_xxx.c */
bool			is_legal_identifier(const char *name);
bool			is_legal_variable_char(char c);
bool			is_legal_variable_starter(char c);
bool			is_legal_number(const char *string, intmax_t *result);

/* libft.c */
int				ft_isalpha(int c);
int				ft_isalnum(int c);
int				ft_isprint(int c);
size_t			ft_strlen(const char *str);
char			*ft_strdup(const char *str);
char			*ft_strchr(const char *str, int c);
void			*ft_memset(void *dst, int c, size_t len);
char			*ft_itoa(int n);
void			ft_itoa_no_alloc(int n, char buf[12]);
ssize_t			ft_putstr_fd(const char *str, int fd);
char			*ft_strjoin(const char *s1, const char *s2);
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void			*ft_memmove(void *dst, const void *src, size_t len);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_strcmp(const char *s1, const char *s2);
// memchr
// strchr
// strndup

/* is_xxx_char.c */
bool			is_blank_char(int c);
bool			is_meta_char(int c);
bool			is_quote_char(int c);

/* is_xxx_elem.c */
bool			is_path_sep_elem(char c);
bool			is_dotdot_elem(const char *path);
bool			is_slash_or_dotsep_elem(const char *path);
bool			is_double_slash_elem(const char *path);
bool			is_directory(const char *path);

/* is_xxx_word.c */
bool			is_quoted_word(t_word *word);

/* canonicalize_path.c */
char			*canonicalize_path(char *path, enum e_status *status);

/* file_utils.c */
bool			file_is_dir(const char *path);
bool			is_same_file(const char *path1, const char *path2);
bool			is_absolute_program(const char *str);
bool			is_valid_fd(int fd);

/* xxx_cwd.c */
char			*update_cwd(const char *for_whom, t_globals *g);
char			*set_cwd(char *cwd, t_globals *g);
char			*get_working_directory(const char *for_whom, \
					enum e_status *status, t_globals *g);
enum e_status	set_working_directory(const char *cwd, t_globals *g);

/* ORGANIZE LATER */
/* extract_colon_unit.c */
char			*extract_colon_unit(const char *string, size_t *index_ptr);
/* substring.c */
char			*substring(const char *string, size_t start, size_t end);
/* join_path.c */
char			*join_path(const char *path, const char *dir, uint8_t flags);
/* join_with_separator.c */
char			*join_with_separator(const char *s1, const char *s2, char sep);
/* make_temp_fd.c */
int				make_temp_fd(const char *root, char **filename, \
				enum e_status *status);
/* general.c */
void			*expand_buffer(void *ptr, size_t size, size_t increase);
bool			is_backslash_newline(const char *str);
/* printable_filename.c */
char			*printable_filename(const char *filename);
/* ft_strtoimax.c */
intmax_t		ft_strtoimax(const char *nptr, char **endptr, int base);

bool			is_escaped_in_dquotes(char c);
char			*strfmt_make_printable(const char *str);
char			*strfmt_double_quote_and_escape(const char *str);
bool			has_unprintable_chars(const char *str);
void			bubble_sort_ptr_array(void **array, t_comp_func func);

// void			print_word_list(t_word_list *list); // remove
#endif /* !GENERAL_H */
