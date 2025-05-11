NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS_DIR = srcs/
SRCS = $(SRCS_DIR)main.c \
       $(SRCS_DIR)input_utils.c \
       $(SRCS_DIR)excute_command.c \
       $(SRCS_DIR)excute_command_utils.c \
       $(SRCS_DIR)redirect_utils.c \
       $(SRCS_DIR)redirect.c \
       $(SRCS_DIR)external_utils.c \
       $(SRCS_DIR)external_commands.c \
       $(SRCS_DIR)pipeline_utils.c \
       $(SRCS_DIR)pipeline.c \
       $(SRCS_DIR)env_expand.c \
       $(SRCS_DIR)env_expand_utils.c \
       $(SRCS_DIR)env_expand_utils2.c \
       $(SRCS_DIR)builtin_commands.c \
       $(SRCS_DIR)builtin_env.c \
       $(SRCS_DIR)builtin_env2.c \
       $(SRCS_DIR)builtin_env3.c \
       $(SRCS_DIR)env_utils.c \
       $(SRCS_DIR)env_utils_extra.c \
       $(SRCS_DIR)tokenizer.c \
       $(SRCS_DIR)tokenizer_utils.c \
       $(SRCS_DIR)parser.c

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
