NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -g -O0

SRCS_DIR = srcs/
SRCS_FILE = main.c \
	builtin_commands.c \
	builtin_export_sort.c \
	builtin_export_utils.c \
	builtin_export.c \
	builtin/builtin_unset.c \
	env_expand_utils.c \
	env_expand_utils2.c \
	env_expand.c \
	env_modify_utils.c \
	env_utils_extra.c \
	env_utils.c \
	excute_command.c \
	execute_builtin.c \
	exit_status.c \
	external/execute_external.c \
	external/external_commands_child_utils.c \
	external/external_commands_find.c \
	external/external_commands_utils.c \
	external/external_commands.c \
	external/external_utils.c \
	heredoc_utils.c \
	heredoc.c \
	input_utils.c \
	parser/parser_token_to_cmd.c \
	parser/parser_tokens.c \
	parser/parser_utils3.c \
	parser/parser.c \
	pipeline_exec.c \
	pipeline_process_utils.c \
	pipeline_setup_utils.c \
	pipeline_utils.c \
	pipeline.c \
	redirect_utils.c \
	redirect.c \
	signal_state.c \
	tokenizer_core_logic.c \
	tokenizer_meta_utils.c \
	tokenizer_normal_word_logic.c \
	tokenizer_quote_processing.c \
	tokenizer_token_handlers.c \
	tokenizer_utils.c \
	tokenizer_utils2.c \
	tokenizer_utils3.c \
	tokenizer.c \
	utils/env_display.c \
	utils/export_utils.c \
	utils/identifier_validator.c
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILE))
OBJS_DIR = obj/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS_FILE:.c=.o))

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) | minishell.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

obj/%.o: $(SRCS_DIR)%.c | obj/
	$(CC) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj/
	mkdir -p obj/external
	mkdir -p obj/parser
	mkdir -p obj/builtin
	mkdir -p obj/utils

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)
fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

cre: clean all

.PHONY: all clean fclean re cre
