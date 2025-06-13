NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -I/usr/include -Ilibft -Isrcs -g -O0

SRCS_DIR = srcs/
SRCS_FILE = main.c \
	builtin/builtin_cd.c \
	builtin/builtin_echo.c \
	builtin/builtin_env.c \
	builtin/builtin_env_print.c \
	builtin/builtin_exit.c \
	builtin/builtin_exit_utils.c \
	builtin/builtin_export_argutils.c \
	builtin/builtin_export.c \
	builtin/builtin_export_set.c \
	builtin/builtin_export_sort.c \
	builtin/builtin_export_utils2.c \
	builtin/builtin_export_utils3.c \
	builtin/builtin_export_utils4.c \
	builtin/builtin_export_utils.c \
	builtin/builtin_pwd.c \
	builtin/builtin_unset.c \
	builtin/execute_builtin.c \
	builtin/execute_builtin_utils.c \
	builtin/identifier_validator.c \
	env/env_expand.c \
	env/env_expand_quote.c \
	env/env_expand_utils2.c \
	env/env_expand_utils.c \
	env/env_utils.c \
	env/env_utils_extra.c \
	env/exit_status.c \
	external/external_commands.c \
	external/external_commands_child_utils.c \
	external/external_commands_exec.c \
	external/external_commands_find.c \
	external/external_commands_utils.c \
	external/external_utils.c \
	parser/heredoc.c \
	parser/heredoc_input.c \
	parser/heredoc_pipe.c \
	parser/heredoc_process.c \
	parser/heredoc_utils2.c \
	parser/heredoc_utils.c \
	parser/parser.c \
	parser/parser_preprocess.c \
	parser/parser_preprocess_space.c \
	parser/parser_preprocess_utils2.c \
	parser/parser_preprocess_utils.c \
	parser/parser_tokens.c \
	parser/parser_token_to_cmd.c \
	parser/parser_utils3.c \
	pipeline/excute_pipeline.c \
	pipeline/pipeline.c \
	pipeline/pipeline_cleanup.c \
	pipeline/pipeline_process_utils.c \
	pipeline/pipeline_setup_utils.c \
	pipeline/pipeline_utils.c \
	redirect/redirect_apply_fd.c \
	redirect/redirect.c \
	redirect/redirect_process.c \
	redirect/redirect_restore.c \
	redirect/redirect_save_fds.c \
	redirect/redirect_setup.c \
	redirect/redirect_utils.c \
	redirect/redirect_validate.c \
	tokenizer/tokenizer.c \
	tokenizer/tokenizer_core_logic.c \
	tokenizer/tokenizer_meta_utils.c \
	tokenizer/tokenizer_normal_word_logic.c \
	tokenizer/tokenizer_quote_processing.c \
	tokenizer/tokenizer_syntax_check.c \
	tokenizer/tokenizer_token_handlers.c \
	tokenizer/tokenizer_utils2.c \
	tokenizer/tokenizer_utils3.c \
	tokenizer/tokenizer_utils4.c \
	tokenizer/tokenizer_utils.c \
	utils/command_handler.c \
	utils/debug_utils.c \
	utils/excute_command.c \
	utils/history_utils.c \
	utils/input_utils.c \
	utils/string_utils.c
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILE))
OBJS_DIR = obj/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS_FILE:.c=.o))

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a

all: ./update_makefile.sh $(LIBFT) $(NAME)

 ./update_makefile.sh:
	$(shell ./update_makefile.sh)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) | minishell.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

#$(OBJS_DIR):
# 	mkdir -p $(OBJS_DIR)
# 	mkdir -p $(OBJS_DIR)builtin
# 	mkdir -p $(OBJS_DIR)env
# 	mkdir -p $(OBJS_DIR)external
# 	mkdir -p $(OBJS_DIR)heredoc
# 	mkdir -p $(OBJS_DIR)parser
# 	mkdir -p $(OBJS_DIR)pipeline
# 	mkdir -p $(OBJS_DIR)redirect
# 	mkdir -p $(OBJS_DIR)tokenizer
# 	mkdir -p $(OBJS_DIR)utils
# 	mkdir -p $(OBJS_DIR)printf

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)
fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

cre: clean all

.PHONY: all clean fclean re cre
