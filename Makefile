NAME					:=	minishell

CC						:=	cc
CFLAGS					=	-Wall -Wextra -MMD -MP $(INCLUDES)
# CFLAGS					=	-Wall -Wextra -MMD -MP $(INCLUDES) -fsanitize=address
LFLAGS					=	-L$(READLINE_PATH)/lib -lreadline
INCLUDES				=	-I$(INCLUDES_PATH) -I$(READLINE_PATH)/include

SRCS_PATH				:=	srcs
OBJS_PATH				:=	objs
INCLUDES_PATH			:=	includes

READLINE_PATH			:=	$(shell brew --prefix readline)

################################################################################
#					      Sources Directory Structure	                       #
################################################################################

SRCS_FILES				:=	main.c read_and_execute_loop.c simple_exec.c print_ast.c

#################### BUILTINS ####################

BUILTINS_PATH			:=	builtins
BUILTINS_FILES			:=	builtin_putstr.c \
							echo_builtin.c \
							exit_builtin.c \
							pwd_builtin.c \
							unset_builtin.c \
							utils_options.c

BUILTIN_CD_PATH			=	$(BUILTINS_PATH)/cd
BUILTIN_CD_FILES		:=	cd_builtin.c \
							change_to_directory.c \
							is_absolute_path.c \
							make_absolute_path.c \
							update_pwd_oldpwd.c

BUILTIN_EXPORT_PATH		=	$(BUILTINS_PATH)/export
BUILTIN_EXPORT_FILES	:=	export_builtin.c \
							export_variable.c \
							show_exported_variables.c

#################### COMMAND ####################

COMMAND_PATH			:=	command
COMMAND_FILES			:=	

COMMAND_COPY_PATH		=	$(COMMAND_PATH)/copy
COMMAND_COPY_FILES		:=	copy_words.c

COMMAND_MAKE_PATH		=	$(COMMAND_PATH)/make
COMMAND_MAKE_FILES		:=	add_to_list.c \
							make_commands.c \
							make_redirection.c \
							make_words.c \
							make_word_list_from_str.c

COMMAND_DESTROY_PATH	=	$(COMMAND_PATH)/destroy
COMMAND_DESTROY_FILES	:=	destroy_command.c \
							destroy_redirects.c \
							destroy_words.c

#################### EXPANSION ####################

EXPANSION_PATH			:=	expansion
EXPANSION_FILES			:=	extract_single_quoted_section.c extract_double_quoted_section.c quote_removal_in_place.c is_ifs.c \
							init_expansion_struct.c expand_word_list.c expand_string_to_string.c string_list.c \
							is_quoted_null.c

EXPAND_WORD_PATH		=	$(EXPANSION_PATH)/expand_word
EXPAND_WORD_FILES		:=	add_xxx_to_expansion.c \
							case_backslash.c \
							case_ctlesc.c \
							case_default.c \
							case_dollar.c \
							case_double_quote.c \
							case_single_quote.c  \
							expand_word.c \
							finish_expansion.c \
							in_dquotes_or_heredoc.c

EXPANSION_QUOTING_PATH	=	$(EXPANSION_PATH)/quote_dequote
EXPANSION_QUOTING_FILES	:=	dequote_escapes_in_place.c \
							dequote_list.c \
							dequote_string_in_place.c \
							quote_escapes.c \
							quote_string.c \
							remove_quoted_ifs_in_place.c \
							remove_quoted_nulls_in_place.c \
							list_remove_quoted_nulls.c

#################### EXECUTE ####################

EXECUTE_PATH			:=	execute
EXECUTE_FILES			:=	search_for_command.c \
							make_argv.c \
							make_envp.c \
							make_child.c \
							jobs.c \
							wait_for.c \
							execute_disk_command.c \
							close_fd_bitmap.c \
							do_piping.c \
							close_pipes.c \
							execute_command.c \
							execute_command_internal.c \
							execute_connection.c \
							execute_pipeline.c \
							execute_subshell_command.c \
							execute_simple_command.c

#################### GENERAL ####################

GENERAL_PATH			:=	general
GENERAL_FILES			:=	canonicalize_path.c \
							extract_colon_unit.c \
							file_utils.c \
							ft_strtoimax.c \
							general.c \
							is_legal_xxx.c \
							is_xxx_char.c \
							is_xxx_elem.c \
							is_xxx_word.c \
							join_path.c \
							join_with_separator.c \
							libft.c \
							make_temp_fd.c \
							printable_filename.c \
							substring.c \
							xxx_cwd.c \
							strfmt_make_printable.c \
							strfmt_double_quote_and_escape.c \
							is_escaped_in_xxx.c \
							has_unprintable_chars.c \
							bubble_sort_ptr_array.c

#################### HASHLIB ####################

HASHLIB_PATH			:=	hashlib
HASHLIB_FILES			:=	hash_bucket.c \
							hash_create.c \
							hash_destroy.c \
							hash_flush.c \
							hash_insert.c \
							hash_remove.c \
							hash_search.c \
							hash_size.c \
							hash_string.c

#################### INIT ####################

INIT_PATH				:=	init
INIT_FILES				:=	import_environment.c \
							init_global_variables.c \
							init_input.c \
							init_minishell.c \
							init_pwd_oldpwd.c \
							init_shell_variables.c

#################### INPUT ####################

INPUT_PATH				:=	input
INPUT_FILES				:=	add_char_to_input_word.c \
							add_to_history_line.c \
							clear_input_line.c \
							readline_get_unget.c \
							shell_getc.c \
							shell_ungetc.c \
							stream_get_unget.c \
							tty_state.c \
							unset_nodelay_mode.c
							

#################### PARSER ####################

PARSER_PATH				:=	parser
PARSER_FILES			:=	handle_parser_error.c \
							is_xxx_token.c \
							parse_command.c \
							parse_connection.c \
							parse_redirection.c \
							parse_simple_element.c \
							parse_subshell.c \
							subshell_stack.c

TOKENIZE_PATH			=	$(PARSER_PATH)/tokenize
TOKENIZE_FILES			:=	add_quoted_section.c \
							get_token_word.c \
							get_token.c

HEREDOC_PATH			=	$(PARSER_PATH)/here_document
HEREDOC_FILES			:=	gather_here_documents.c \
							push_here_document.c

#################### REDIRECTION ####################

REDIRECTION_PATH		:=	redirection
REDIRECTION_FILES		:=	backup_fd.c \
							do_redirections.c \
							do_xxx_redirection.c \
							has_stdin_redirects.c \
							heredoc_to_fd.c \
							is_xxx_redirection.c \
							redirection_expand.c

#################### SIGNAL ####################

SIGNAL_PATH				:=	signal
SIGNAL_FILES			:=	prompt_signals.c \
							set_signal_handler.c \
							child_signals.c \
							parent_signals.c


#################### VARIABLES ####################

VARIABLES_PATH			:=	variables
VARIABLES_FILES			:=	adjust_shell_level.c \
							bind_variable_internal.c \
							bind_variable.c \
							create_variable_table.c \
							destroy_var_context.c \
							destroy_variable.c \
							find_variable.c \
							free_variable_hash_data.c \
							get_string_value.c \
							handle_if_special_variable_modified.c \
							hash_lookup.c \
							is_xxx_var.c \
							make_variable_value.c \
							new_var_context.c \
							new_variable.c \
							set_if_not.c \
							set_ifs.c \
							unbind_variable.c \
							var_lookup.c \
							make_variable_array.c

#################### WRAPPERS ####################

WRAPPERS_PATH			:=	wrappers
WRAPPERS_FILES			:=	xaccess.c \
							xchdir.c \
							xclose.c \
							xclosedir.c \
							xdup.c \
							xdup2.c \
							xexecve.c \
							xexit.c \
							xfork.c \
							xfree.c \
							xfstat.c \
							xgetcwd.c \
							xgetenv.c \
							xioctl.c \
							xisatty.c \
							xkill.c \
							xlstat.c \
							xmalloc.c \
							xopen.c \
							xopendir.c \
							xperror.c \
							xpipe.c \
							xread.c \
							xreaddir.c \
							xsigaction.c \
							xsigaddset.c \
							xsigemptyset.c \
							xsignal.c \
							xstat.c \
							xstrerror.c \
							xtcgetattr.c \
							xtcsetattr.c \
							xttyname.c \
							xunlink.c \
							xwait.c \
							xwait3.c \
							xwait4.c \
							xwaitpid.c \
							xwrite.c \

################################################################################
#					               File Patterns	                           #
################################################################################

SRCS					=	$(SRCS_FILES) \
							$(addprefix $(BUILTINS_PATH)/, $(BUILTINS_FILES)) \
							$(addprefix $(BUILTIN_CD_PATH)/, $(BUILTIN_CD_FILES)) \
							$(addprefix $(BUILTIN_EXPORT_PATH)/, $(BUILTIN_EXPORT_FILES)) \
							$(addprefix $(COMMAND_COPY_PATH)/, $(COMMAND_COPY_FILES)) \
							$(addprefix $(COMMAND_MAKE_PATH)/, $(COMMAND_MAKE_FILES)) \
							$(addprefix $(COMMAND_DESTROY_PATH)/, $(COMMAND_DESTROY_FILES)) \
							$(addprefix $(EXPANSION_PATH)/, $(EXPANSION_FILES)) \
							$(addprefix $(EXPAND_WORD_PATH)/, $(EXPAND_WORD_FILES)) \
							$(addprefix $(EXPANSION_QUOTING_PATH)/, $(EXPANSION_QUOTING_FILES)) \
							$(addprefix $(EXECUTE_PATH)/, $(EXECUTE_FILES)) \
							$(addprefix $(GENERAL_PATH)/, $(GENERAL_FILES)) \
							$(addprefix $(HASHLIB_PATH)/, $(HASHLIB_FILES)) \
							$(addprefix $(INIT_PATH)/, $(INIT_FILES)) \
							$(addprefix $(INPUT_PATH)/, $(INPUT_FILES)) \
							$(addprefix $(PARSER_PATH)/, $(PARSER_FILES)) \
							$(addprefix $(TOKENIZE_PATH)/, $(TOKENIZE_FILES)) \
							$(addprefix $(HEREDOC_PATH)/, $(HEREDOC_FILES)) \
							$(addprefix $(REDIRECTION_PATH)/, $(REDIRECTION_FILES)) \
							$(addprefix $(SIGNAL_PATH)/, $(SIGNAL_FILES)) \
							$(addprefix $(VARIABLES_PATH)/, $(VARIABLES_FILES)) \
							$(addprefix $(WRAPPERS_PATH)/, $(WRAPPERS_FILES))

OBJS					=	$(addprefix $(OBJS_PATH)/, $(SRCS:.c=.o))

DEPS					=	$(OBJS:%.o=%.d)

################################################################################
#					           General Rules	                               #
################################################################################

.PHONY: all clean fclean re

#################### BUILD ####################

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJS) $(UNIT_TEST_FILES) -o $@

$(OBJS_PATH)/%.o: $(SRCS_PATH)/%.c | $(OBJS_PATH)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_PATH):
	mkdir -p $@

-include $(DEPS)

#################### CLEAN ####################

clean:
	rm -rf $(OBJS_PATH)
	rm -rf $(OBJS_UNIT_TEST_PATH)

fclean: clean
	rm -f $(NAME)

#################### REBUILD ####################

re: fclean all

################################################################################
#					              Unit Testing	                               #
################################################################################

.PHONY: unit

OBJS_UNIT_TEST_PATH		:=	objs_unit_test
UNITY_MEMORY_PATH		:=	unit-tests/Unity/extras/memory/src

OBJS_UNIT_TEST			=	$(addprefix $(OBJS_UNIT_TEST_PATH)/, $(SRCS:.c=.o))
DEPS_UNIT				=	$(OBJS_UNIT_TEST:%.o=%.d)

unit: $(OBJS_UNIT_TEST)

$(OBJS_UNIT_TEST_PATH)/%.o: $(SRCS_PATH)/%.c | $(OBJS_UNIT_TEST_PATH)
	mkdir -p $(dir $@)
	$(CC) -Wall -Wextra -MMD -MP $(INCLUDES) -fsanitize=address -DUNIT_TESTING=1 -I$(UNITY_MEMORY_PATH) -c $< -o $@

$(OBJS_UNIT_TEST_PATH):
	mkdir -p $@

-include $(DEPS_UNIT)

################################################################################
#					                   Misc	                                   #
################################################################################

.PHONY: doxy norm nm help

DOXYGEN_PATH			=	doxygen
DOXYFILE				=	Doxyfile

doxy:
	cd $(DOXYGEN_PATH) && doxygen $(DOXYFILE)

norm:
	-norminette $(SRCS_PATH)
	-norminette $(INCLUDES_PATH)

nm:
	nm -u minishell | grep -v -E \
	'readline|rl_clear_history|rl_on_new_line|rl_replace_line|rl_redisplay|\
	add_history|malloc|free|write|access|open|read|close|fork|wait|\
	waitpid|wait3|wait4|signal|sigaction|sigemptyset|sigaddset|kill|exit|\
	getcwd|chdir|stat|lstat|fstat|unlink|execve|dup|dup2|pipe|opendir|\
	readdir|closedir|strerror|perror|isatty|ttyname|ttyslot|ioctl|getenv|\
	tcsetattr|tcgetattr|tgetent|tgetflag|tgetnum|tgetstr|tgoto|tputs|rl_'

# nm:
# 	nm -u minishell | grep -v -E \
# 	'readline|rl_clear_history|rl_on_new_line|rl_replace_line|rl_redisplay|\
# 	add_history|printf|malloc|free|write|access|open|read|close|fork|wait|\
# 	waitpid|wait3|wait4|signal|sigaction|sigemptyset|sigaddset|kill|exit|\
# 	getcwd|chdir|stat|lstat|fstat|unlink|execve|dup|dup2|pipe|opendir|\
# 	readdir|closedir|strerror|perror|isatty|ttyname|ttyslot|ioctl|getenv|\
# 	tcsetattr|tcgetattr|tgetent|tgetflag|tgetnum|tgetstr|tgoto|tputs'

help:
	@echo	"help:		Show this help."
	@echo
	@echo	"all:		Build the minishell executable."
	@echo	"clean:		Remove all object files."
	@echo	"fclean:		Remove all object files and the minishell executable."
	@echo	"re:		Recompile all object files and rebuild the minishell executable."
	@echo
	@echo	"norm:		Run norminette on all source files and header files."
	@echo	"nm:		Run "nm -u" on the minishell executable, excluding allowed functions."
	@echo	"doxy:		Regenerate doxygen documentation."
	@echo	"unit:		Compile object files with unity_malloc() and unity_free() in $(OBJS_UNIT_TEST_PATH)."
