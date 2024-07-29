#ifndef TOKENIZE_INTERNAL_H
# define TOKENIZE_INTERNAL_H

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

/* get_token_word.c */
void			get_token_word(int c, t_input *input);

/* add_quoted_section.c */
void			add_quoted_section(int quote, t_input *input);

#endif /* !TOKENIZE_INTERNAL_H */
