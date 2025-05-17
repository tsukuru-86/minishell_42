NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -g -O0 -std=c99
# CFLAGS = -Wall -Wextra -Werror -I. -g -O0 -fsanitize=address,undefined

SRCS_DIR = srcs/
SRCS_FILE = main.c \
	builtin_cmds.c \
	builtin_env.c \
	builtin_env2.c \
	builtin_env3.c \
	builtin_utils.c \
	env_array_utils.c \
	env_expand_init.c \
	env_expand_utils.c \
	env_expand.c \
	env_list_helper2.c \
	env_list_to_array.c \
	env_list.c \
	env_modification_removers.c \
	env_modification_setters.c \
	env_utils_create.c \
	env_utils_helpers.c \
	env_utils_validation.c \
	env_utils.c \
	execute_command_utils.c \
	execute_command_utils2.c \
	execute_command.c \
	external_commands.c \
	ft_printf_fd_utils.c \
	ft_printf_fd.c \
	input_utils.c \
	parser_command_ops.c \
	parser_env_utils.c \
	parser_helpers.c \
	parser_memory.c \
	parser_redirect_processing.c \
	parser_token_processing.c \
	parser_utils.c \
	parser.c \
	pipeline_cleanup.c \
	pipeline_execution.c \
	pipeline_setup.c \
	pipeline.c \
	redirect_utils.c \
	redirect.c \
	shell_state.c \
	signal_handler.c \
	signal_handlers.c \
	signal_state.c \
	token_creator.c \
	token_processing.c \
	tokenizer_utils_checks.c \
	tokenizer_utils_handlers.c \
	tokenizer_utils_meta.c \
	tokenizer_utils_quote.c \
	tokenizer_utils.c \
	tokenizer.c
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILE))
OBJS = $(addprefix obj/, $(SRCS_FILE:.c=.o))

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) | minishell.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

obj/%.o: $(SRCS_DIR)%.c | obj/
	$(CC) $(CFLAGS) -c $< -o $@

obj/:
	mkdir -p obj/

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

cre: clean all

.PHONY: all clean fclean re cre
