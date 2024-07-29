#ifndef BUILTINS_INTERNAL_H
# define BUILTINS_INTERNAL_H

# include <stdbool.h>

# include "command.h"

/* **************************************************************** */
/*	                          Defines	                            */
/* **************************************************************** */

# define INVALID_ID_ERR_FORMAT "minishell: %s: `%s': not a valid identifier\n"

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

/* builtin_putsr.c */
enum e_status	builtin_putstr(const char *str, const char *builtin);
enum e_status	builtin_putstr_nl(const char *str, const char *builtin);
enum e_status	builtin_put_assign_nl(const char *name, const char *value, \
				const char *builtin);

/* utils_options.c */
bool			is_help_option(const char *str);
bool			is_end_of_options(const char *word);
int				check_options(t_word_list **list, const char *name, \
				const char *usage, const char *longdoc);
void			print_usage(const char *name, const char *usage, \
				const char *longdoc);

/* **************************************************************** */
/*	                        Usage Messages	                        */
/* **************************************************************** */

# define CD_USAGE		"cd [dir]"
# define ECHO_USAGE		"echo [-n] [arg ...]"
# define EXIT_USAGE 	"exit [n]"
# define EXPORT_USAGE	"export [name[=value] ...]"
# define PWD_USAGE 		"pwd"
# define UNSET_USAGE	"unset [name ...]"

# define CD_LONGDOC "\
    Change the shell working directory.\n\
    \n\
    Change the current directory to DIR.  The default DIR is the value of\n\
    the HOME shell variable. If DIR is \"-\", it is converted to $OLDPWD.\n\
    \n\
    The variable CDPATH defines the search path for the directory\n\
    containing DIR.  Alternative directory names in CDPATH are separated\n\
    by a colon (:).\n\
    A null directory name is the same as the current directory.  If DIR\n\
    begins with a slash (/), then CDPATH is not used.\n\
    \n\
    Symbolic links in DIR are processed after processing instances of `..'.\n\
    `..' is processed by removing the immediately previous pathname\n\
    component back to a slash or the beginning of DIR.\n\
    \n\
    Exit Status:\n\
    Returns 0 if the directory is changed, non-zero otherwise."

# define ECHO_LONGDOC "\
    Write arguments to the standard output.\n\
    \n\
    Display the ARGs on the standard output followed by a newline.\n\
    \n\
    Options:\n\
      -n	do not append a newline\n\
    \n\
    Exit Status:\n\
    Returns success unless a write error occurs."

# define EXIT_LONGDOC "\
    Exit the shell.\n\
    \n\
    Exits the shell with a status of N.  If N is omitted, the exit status\n\
    is that of the last command executed."

# define EXPORT_LONGDOC "\
    Set export attribute for shell variables.\n\
    \n\
    Marks each NAME for automatic export to the environment of subsequently\n\
    executed commands.  If VALUE is supplied, assign VALUE before exporting.\n\
    \n\
    An argument of `--' disables further option processing.\n\
    \n\
    Exit Status:\n\
    Returns success unless an invalid option is given or NAME is invalid."

# define PWD_LONGDOC "\
    Print the name of the current working directory.\n\
    \n\
    Prints the value of $PWD if it names the current working\n\
    directory.\n\
    \n\
    Exit Status:\n\
    Returns 0 unless an invalid option is given or the current directory\n\
    cannot be read."

# define UNSET_LONGDOC "\
    Unset values and attributes of shell variables.\n\
    \n\
    For each NAME, remove the corresponding variable.\n\
    \n\
    Exit Status:\n\
    Returns success unless an invalid option is given."

#endif /* !BUILTINS_INTERNAL_H */
