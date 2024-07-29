#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

/* heredoc/gather_here_documents.c */
void			gather_here_documents(t_parser *parser);

/* heredoc/push_here_document.c */
void			push_here_document(t_parser *parser);

/* tokenize/get_token.c */
enum e_token	get_token(t_parser *parser);

/* handle_parser_error.c */
void			handle_parser_error(t_parser *parser);

/* is_xxx_token.c */
bool			is_redirection_token(enum e_token token);
bool			is_connection_token(enum e_token token);
bool			is_end_of_command_token(enum e_token token);

/* parse_connection.c */
void			parse_connection(t_parser *parser);

/* parse_redirection.c */
void			parse_redirection(t_parser *parser);

/* parse_simple_element.c */
void			parse_simple_element(t_parser *parser);

/* parse_subshell.c */
void			parse_subshell_start(t_parser *parser);
void			parse_subshell_end(t_parser *parser);

#endif /* !PARSER_INTERNAL_H */
