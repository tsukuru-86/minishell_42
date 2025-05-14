NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I.

SRCS_DIR = srcs/
SRCS = $(SRCS_DIR)main.c \
		$(SRCS_DIR)builtin_commands.c \
		$(SRCS_DIR)builtin_env.c \
		$(SRCS_DIR)builtin_env2.c \
		$(SRCS_DIR)builtin_env3.c \
		$(SRCS_DIR)env_array_utils.c \
		$(SRCS_DIR)env_expand_utils.c \
		$(SRCS_DIR)env_expand.c \
		$(SRCS_DIR)env_list.c \
		$(SRCS_DIR)env_list_to_array.c \
		$(SRCS_DIR)env_list_helper2.c \
		$(SRCS_DIR)env_utils_access.c \
		$(SRCS_DIR)env_utils_helpers.c \
		$(SRCS_DIR)env_utils_create.c \
		$(SRCS_DIR)env_utils_validation.c \
		$(SRCS_DIR)env_modification_setters.c \
		$(SRCS_DIR)env_modification_removers.c \
		$(SRCS_DIR)excute_command_utils.c \
		$(SRCS_DIR)excute_command.c \
		$(SRCS_DIR)external_commands.c \
		$(SRCS_DIR)external_utils.c \
		$(SRCS_DIR)input_utils.c \
		$(SRCS_DIR)parser.c \
		$(SRCS_DIR)parser_env_utils.c \
		$(SRCS_DIR)parser_helpers.c \
		$(SRCS_DIR)parser_command_ops.c \
		$(SRCS_DIR)parser_token_handlers.c \
		$(SRCS_DIR)parser_token_processing.c \
		$(SRCS_DIR)parser_memory.c \
		$(SRCS_DIR)parser_redirect_utils.c \
		$(SRCS_DIR)pipeline_setup.c \
		$(SRCS_DIR)pipeline_execution.c \
		$(SRCS_DIR)pipeline_cleanup.c \
		$(SRCS_DIR)pipeline_utils.c \
		$(SRCS_DIR)redirect_utils.c \
		$(SRCS_DIR)redirect.c \
		$(SRCS_DIR)tokenizer_utils_checks.c \
		$(SRCS_DIR)tokenizer_utils_meta.c \
		$(SRCS_DIR)tokenizer.c \
		$(SRCS_DIR)tokenizer_handlers.c \
		$(SRCS_DIR)tokenizer_utils.c \
		$(SRCS_DIR)ft_printf_fd.c \
		$(SRCS_DIR)ft_printf_fd_utils.c \
		$(SRCS_DIR)redir_utils_conversion.c \
		$(SRCS_DIR)token_creator.c \
		$(SRCS_DIR)token_processing.c \
		$(SRCS_DIR)parser_redirect_processing.c \
		$(SRCS_DIR)parser_utils.c

# OBJS = $(addprefix obj/, $(SRCS:.c=.o))
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) | minishell.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
