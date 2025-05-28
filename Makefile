NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -I/usr/include -Ilibft -Isrcs -g -O0

SRCS_DIR = srcs/
SRCS_FILE = main.c \
	builtin/builtin_export.c \
	builtin/builtin_export_set.c \
	builtin/builtin_export_sort.c \
	builtin/builtin_export_utils.c \
	builtin/builtin_others.c \
	builtin/builtin_unset.c \
	builtin/execute_builtin.c \
	env/env_expand.c \
	env/env_expand_utils.c \
	env/env_expand_utils2.c \
	env/env_utils.c \
	env/env_utils_extra.c \
	external/execute_external.c \
	external/external_commands.c \
	external/external_commands_child_utils.c \
	external/external_commands_find.c \
	external/external_commands_utils.c \
	heredoc/heredoc.c \
	heredoc/heredoc_utils.c \
	parser/parser.c \
	parser/parser_token_to_cmd.c \
	parser/parser_tokens.c \
	parser/parser_utils3.c \
	pipeline/excute_pipeline.c \
	pipeline/pipeline.c \
	pipeline/pipeline_process_utils.c \
	pipeline/pipeline_setup_utils.c \
	pipeline/pipeline_utils.c \
	redirect/redirect.c \
	redirect/redirect_utils.c \
	tokenizer/tokenizer.c \
	tokenizer/tokenizer_core_logic.c \
	tokenizer/tokenizer_meta_utils.c \
	tokenizer/tokenizer_normal_word_logic.c \
	tokenizer/tokenizer_quote_processing.c \
	tokenizer/tokenizer_token_handlers.c \
	tokenizer/tokenizer_utils.c \
	tokenizer/tokenizer_utils2.c \
	tokenizer/tokenizer_utils3.c \
	utils/excute_command.c \
	utils/exit_status.c \
	utils/identifier_validator.c \
	utils/input_utils.c \
	utils/signal_state.c
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILE))
OBJS_DIR = obj/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS_FILE:.c=.o))

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a

all: $(OBJS_DIR) $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) | minishell.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)
	mkdir -p $(OBJS_DIR)builtin
	mkdir -p $(OBJS_DIR)env
	mkdir -p $(OBJS_DIR)external
	mkdir -p $(OBJS_DIR)heredoc
	mkdir -p $(OBJS_DIR)parser
	mkdir -p $(OBJS_DIR)pipeline
	mkdir -p $(OBJS_DIR)redirect
	mkdir -p $(OBJS_DIR)tokenizer
	mkdir -p $(OBJS_DIR)utils

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)
fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

cre: clean all

.PHONY: all clean fclean re cre
