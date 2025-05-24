NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -I/usr/include -Ilibft -Isrcs -g -O0

SRCS_DIR = srcs/
SRCS_FILE = main.c \
	builtin/builtin_export_set.c \
	builtin/builtin_export_sort.c \
	builtin/builtin_export_utils.c \
	builtin/builtin_export.c \
	builtin/builtin_others.c \
	builtin/builtin_unset.c \
	builtin/execute_builtin.c \
	env/env_expand_utils.c \
	env/env_expand_utils2.c \
	env/env_expand.c \
	env/env_utils_extra.c \
	env/env_utils.c \
	external/execute_external.c \
	external/external_commands_child_utils.c \
	external/external_commands_find.c \
	external/external_commands_utils.c \
	external/external_commands.c \
	external/external_utils.c \
	heredoc/heredoc_utils.c \
	heredoc/heredoc.c \
	parser/parser_token_to_cmd.c \
	parser/parser_tokens.c \
	parser/parser_utils3.c \
	parser/parser.c \
	pipeline/excute_pipeline.c \
	pipeline/pipeline_process_utils.c \
	pipeline/pipeline_setup_utils.c \
	pipeline/pipeline_utils.c \
	pipeline/pipeline.c \
	redirect/redirect_utils.c \
	redirect/redirect.c \
	tokenizer/tokenizer_core_logic.c \
	tokenizer/tokenizer_meta_utils.c \
	tokenizer/tokenizer_normal_word_logic.c \
	tokenizer/tokenizer_quote_processing.c \
	tokenizer/tokenizer_token_handlers.c \
	tokenizer/tokenizer_utils.c \
	tokenizer/tokenizer_utils2.c \
	tokenizer/tokenizer_utils3.c \
	tokenizer/tokenizer.c \
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

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) | minishell.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

obj/%.o: $(SRCS_DIR)%.c | obj/
	$(CC) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj/
	mkdir -p obj/builtin
	mkdir -p obj/env
	mkdir -p obj/external
	mkdir -p obj/heredoc
	mkdir -p obj/parser
	mkdir -p obj/pipeline
	mkdir -p obj/redirect
	mkdir -p obj/tokenizer
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
