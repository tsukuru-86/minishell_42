NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -I/usr/include -Ilibft -Isrcs -g -O0

# OS検出と環境依存の設定
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # macOS固有のフラグを追加
    CFLAGS += -DMACOS
    $(info MacOS環境を検出しました。readline関数の互換性対応を有効化します。)
endif
ifeq ($(UNAME_S),Linux)
    # Linux固有のフラグを追加
    CFLAGS += -DLINUX
    $(info Linux環境を検出しました。標準のreadline関数を使用します。)
endif

SRCS_DIR = srcs/
SRCS_FILE = main.c \
	builtin/builtin_cd.c \
	builtin/builtin_echo.c \
	builtin/builtin_env.c \
	builtin/builtin_env_print.c \
	builtin/builtin_exit.c \
	builtin/builtin_exit_error.c \
	builtin/builtin_exit_utils.c \
	builtin/builtin_export_argutils.c \
	builtin/builtin_export.c \
	builtin/builtin_export_print.c \
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
	external/external_commands_exec2.c \
	external/external_commands_exec.c \
	external/external_commands_find.c \
	external/external_commands_utils.c \
	external/external_utils.c \
	main_loop.c \
	parser/heredoc.c \
	parser/heredoc_input.c \
	parser/heredoc_pipe.c \
	parser/heredoc_process.c \
	parser/heredoc_utils2.c \
	parser/heredoc_utils.c \
	parser/parser_args.c \
	parser/parser.c \
	parser/parser_heredoc_lookahead.c \
	parser/parser_preprocess.c \
	parser/parser_preprocess_space.c \
	parser/parser_preprocess_utils.c \
	parser/parser_token_handlers.c \
	parser/parser_token_merge.c \
	parser/parser_token_remove.c \
	parser/parser_tokens.c \
	parser/parser_token_to_cmd.c \
	parser/parser_token_utils2.c \
	parser/parser_token_utils3.c \
	parser/parser_token_utils.c \
	parser/parser_utils2.c \
	parser/parser_utils.c \
	pipeline/excute_pipeline.c \
	pipeline/pipeline.c \
	pipeline/pipeline_cleanup.c \
	pipeline/pipeline_process_utils2.c \
	pipeline/pipeline_process_utils.c \
	pipeline/pipeline_setup_utils.c \
	pipeline/pipeline_utils.c \
	redirect/redirect_apply_fd.c \
	redirect/redirect.c \
	redirect/redirect_file_check.c \
	redirect/redirect_process.c \
	redirect/redirect_process_helpers.c \
	redirect/redirect_restore.c \
	redirect/redirect_save_fds.c \
	redirect/redirect_setup.c \
	redirect/redirect_utils2.c \
	redirect/redirect_utils.c \
	redirect/redirect_validate.c \
	tokenizer/tokenizer.c \
	tokenizer/tokenizer_core_logic.c \
	tokenizer/tokenizer_input_processing.c \
	tokenizer/tokenizer_meta_utils.c \
	tokenizer/tokenizer_normal_word_logic.c \
	tokenizer/tokenizer_pipe_redirect_fix.c \
	tokenizer/tokenizer_quote_processing.c \
	tokenizer/tokenizer_syntax_check_advanced.c \
	tokenizer/tokenizer_syntax_check.c \
	tokenizer/tokenizer_token_handlers.c \
	tokenizer/tokenizer_utils2.c \
	tokenizer/tokenizer_utils3.c \
	tokenizer/tokenizer_utils4.c \
	tokenizer/tokenizer_utils.c \
	utils/command_handler.c \
	utils/debug2.c \
	utils/debug.c \
	utils/debug_int.c \
	utils/debug_utils.c \
	utils/empty_command_handler.c \
	utils/empty_input_handler.c \
	utils/excute_command.c \
	utils/history_utils.c \
	utils/history_utils_linux.c \
	utils/input_limits2.c \
	utils/input_limits.c \
	utils/line_utils.c \
	utils/non_interactive_utils.c \
	utils/readline_utils.c \
	utils/string_utils.c
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILE))
OBJS_DIR = obj/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS_FILE:.c=.o))

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a

# ディレクトリ作成の最適化用フラグファイル（隠しファイル）
OBJDIR_FLAG := $(OBJS_DIR).builddir_ready


all: check_makefile_update $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) | minishell.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

# ディレクトリ作成の最適化：一度だけ必要なディレクトリを作成
$(OBJDIR_FLAG):
	@mkdir -p $(sort $(dir $(OBJS)))
	@touch $@

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c $(OBJDIR_FLAG) | minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re cre check_makefile_update

# ソースファイル変更の自動検知
check_makefile_update:
	@if [ -n "$$(find $(SRCS_DIR) -name '*.c' -newer Makefile 2>/dev/null | head -1)" ]; then \
		echo "ソースファイルの変更を検知しました。Makefileを自動更新中..."; \
		./update_makefile.sh; \
	fi

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

cre: clean all

test1: cre
	DEBUG=0
	cd ~/42/minishell_42/minishell_tester-nda-cunh&&./tester 2>&1

test2: cre
	cd ~/42/minishell_42/minishell_tester&&./tester 2>&1

debug: 
	$(MAKE) cre CFLAGS="$(CFLAGS) -DDEBUG=1"

.PHONY: all clean fclean re cre test1 test2  debug
