NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -g -O0

SRCS_DIR = srcs/
SRCS_FILE = main.c \
	builtin_commands.c \
	builtin_env.c \
	builtin_export_utils.c \
	builtin_export.c \
	env_expand_utils.c \
	env_expand.c \
	env_utils_extra.c \
	env_utils.c \
	excute_command_utils.c \
	excute_command.c \
	execute_builtin.c \
	execute_external.c \
	external_commands.c \
	external_utils.c \
	input_utils.c \
	parser.c \
	pipeline_utils.c \
	pipeline.c \
	redirect_utils.c \
	redirect.c \
	signal_state.c \
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
