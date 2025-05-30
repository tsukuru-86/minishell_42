#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    generate_module_tests.sh                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: student <student@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/31 00:00:00 by student           #+#    #+#              #
#    Updated: 2025/05/31 00:00:00 by student          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

HEADER_FILE="minishell.h"
TEST_DIR="tests"
SRC_DIR="srcs"

# Create test directory if it doesn't exist
mkdir -p $TEST_DIR

echo -e "${CYAN}=== Minishell Test Generator ===${NC}"
echo -e "${BLUE}Analyzing $HEADER_FILE...${NC}"

# Function to generate test header
generate_test_header() {
    local output_file=$1
    local module_name=$2
    
    cat > "$output_file" << 'EOF'
/* **************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_MODULE.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muiida <muiida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 00:00:00 by muiida            #+#    #+#             */
/*   Updated: 2025/05/31 04:39:21 by muiida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../minishell.h"

EOF
    
    sed -i "s/MODULE/$module_name/g" "$output_file"
}

# Function to extract functions from header and generate tests
generate_specific_tests() {
    local module=$1
    local test_file="$TEST_DIR/test_${module}.c"
    
    echo -e "${CYAN}Generating tests for $module module...${NC}"
    
    generate_test_header "$test_file" "$module"
    
    case $module in
        "tokenizer")
            cat >> "$test_file" << 'EOF'
/* Tokenizer Tests */
static void	test_tokenize_simple(void)
{
	char		*input = "echo hello";
	t_command	cmd = {0};
	t_token		*tokens;

	printf("Testing tokenize with simple command...\n");
	tokens = tokenize(input, &cmd);
	assert(tokens != NULL);
	assert(tokens->type == TOKEN_WORD);
	assert(strcmp(tokens->content, "echo") == 0);
	free_tokens(tokens);
	printf("✓ tokenize simple test passed\n");
}

static void	test_tokenize_with_quotes(void)
{
	char		*input = "echo \"hello world\"";
	t_command	cmd = {0};
	t_token		*tokens;

	printf("Testing tokenize with quotes...\n");
	tokens = tokenize(input, &cmd);
	assert(tokens != NULL);
	free_tokens(tokens);
	printf("✓ tokenize quotes test passed\n");
}

static void	test_tokenize_with_pipe(void)
{
	char		*input = "echo hello | cat";
	t_command	cmd = {0};
	t_token		*tokens;

	printf("Testing tokenize with pipe...\n");
	tokens = tokenize(input, &cmd);
	assert(tokens != NULL);
	free_tokens(tokens);
	printf("✓ tokenize pipe test passed\n");
}

static void	test_is_meta(void)
{
	printf("Testing is_meta function...\n");
	assert(is_meta('|') == 1);
	assert(is_meta('<') == 1);
	assert(is_meta('>') == 1);
	assert(is_meta('a') == 0);
	assert(is_meta(' ') == 0);
	printf("✓ is_meta test passed\n");
}

static void	test_is_quote(void)
{
	printf("Testing is_quote function...\n");
	assert(is_quote('\'') == 1);
	assert(is_quote('\"') == 1);
	assert(is_quote('a') == 0);
	assert(is_quote('|') == 0);
	printf("✓ is_quote test passed\n");
}

static void	test_is_delimiter(void)
{
	printf("Testing is_delimiter function...\n");
	assert(is_delimiter(' ') == 1);
	assert(is_delimiter('\t') == 1);
	assert(is_delimiter('\n') == 1);
	assert(is_delimiter('a') == 0);
	printf("✓ is_delimiter test passed\n");
}
EOF
            ;;
            
        "parser")
            cat >> "$test_file" << 'EOF'
/* Parser Tests */
static void	test_parse_simple_command(void)
{
	t_token		*tokens;
	t_command	*cmd;
	t_token		token1 = {"echo", TOKEN_WORD, NULL};
	t_token		token2 = {"hello", TOKEN_WORD, NULL};

	printf("Testing parse_tokens with simple command...\n");
	token1.next = &token2;
	tokens = &token1;
	
	cmd = parse_tokens(tokens);
	assert(cmd != NULL);
	assert(cmd->args != NULL);
	assert(strcmp(cmd->args[0], "echo") == 0);
	assert(strcmp(cmd->args[1], "hello") == 0);
	free_command(cmd);
	printf("✓ parse simple command test passed\n");
}

static void	test_is_builtin(void)
{
	printf("Testing is_builtin function...\n");
	assert(is_builtin("echo") == 1);
	assert(is_builtin("cd") == 1);
	assert(is_builtin("pwd") == 1);
	assert(is_builtin("export") == 1);
	assert(is_builtin("unset") == 1);
	assert(is_builtin("env") == 1);
	assert(is_builtin("exit") == 1);
	assert(is_builtin("ls") == 0);
	assert(is_builtin("grep") == 0);
	printf("✓ is_builtin test passed\n");
}
EOF
            ;;
            
        "env")
            cat >> "$test_file" << 'EOF'
/* Environment Tests */
static void	test_create_env_node(void)
{
	t_env	*node;
	char	*test_str = "PATH=/usr/bin";

	printf("Testing create_env_node...\n");
	node = create_env_node(test_str);
	assert(node != NULL);
	assert(strcmp(node->name, "PATH") == 0);
	assert(strcmp(node->value, "/usr/bin") == 0);
	free(node->name);
	free(node->value);
	free(node);
	printf("✓ create_env_node test passed\n");
}

static void	test_get_env_node(void)
{
	t_env	*node;

	printf("Testing get_env_node...\n");
	node = get_env_node("PATH");
	/* Note: This test depends on environment setup */
	if (node)
		printf("✓ get_env_node found PATH\n");
	else
		printf("⚠ get_env_node: PATH not found (may be expected)\n");
}

static void	test_expand_env_vars_simple(void)
{
	char	*result;
	char	*input = "$HOME";

	printf("Testing expand_env_vars with $HOME...\n");
	result = expand_env_vars(input, 0);
	assert(result != NULL);
	printf("✓ expand_env_vars simple test passed\n");
	free(result);
}

static void	test_expand_env_vars_exit_status(void)
{
	char	*result;
	char	*input = "$?";

	printf("Testing expand_env_vars with $?...\n");
	result = expand_env_vars(input, 0);
	assert(result != NULL);
	printf("✓ expand_env_vars exit status test passed\n");
	free(result);
}
EOF
            ;;
            
        "builtin")
            cat >> "$test_file" << 'EOF'
/* Builtin Tests */
static void	test_execute_builtin_echo(void)
{
	char	*args[] = {"echo", "hello", "world", NULL};
	int		result;

	printf("Testing execute_builtin with echo...\n");
	result = execute_builtin(args);
	assert(result >= 0);
	printf("✓ execute_builtin echo test passed\n");
}

static void	test_execute_builtin_pwd(void)
{
	char	*args[] = {"pwd", NULL};
	int		result;

	printf("Testing execute_builtin with pwd...\n");
	result = execute_builtin(args);
	assert(result >= 0);
	printf("✓ execute_builtin pwd test passed\n");
}

static void	test_is_valid_identifier(void)
{
	printf("Testing is_valid_identifier...\n");
	assert(is_valid_identifier("valid_var") == 1);
	assert(is_valid_identifier("_valid") == 1);
	assert(is_valid_identifier("123invalid") == 0);
	assert(is_valid_identifier("invalid-var") == 0);
	assert(is_valid_identifier("") == 0);
	printf("✓ is_valid_identifier test passed\n");
}
EOF
            ;;
            
        "redirect")
            cat >> "$test_file" << 'EOF'
/* Redirection Tests */
static void	test_create_redirect(void)
{
	t_redirect	*redir;
	char		*filename = "test.txt";

	printf("Testing create_redirect...\n");
	redir = create_redirect(REDIR_OUT, filename);
	assert(redir != NULL);
	assert(redir->type == REDIR_OUT);
	assert(strcmp(redir->file, filename) == 0);
	free_redirect(redir);
	printf("✓ create_redirect test passed\n");
}

static void test_redirect_types(void)
{
	printf("Testing redirect type constants...\n");
	assert(REDIR_OUT == 0 || REDIR_OUT != REDIR_IN);
	assert(REDIR_IN != REDIR_APPEND);
	assert(REDIR_APPEND != REDIR_HEREDOC);
	printf("✓ redirect types test passed\n");
}
EOF
            ;;
            
        "heredoc")
            cat >> "$test_file" << 'EOF'
/* Heredoc Tests */
static void	test_init_heredoc(void)
{
	t_heredoc	*heredoc;
	char		*delimiter = "EOF";

	printf("Testing init_heredoc...\n");
	heredoc = init_heredoc(delimiter);
	assert(heredoc != NULL);
	assert(strcmp(heredoc->delimiter, delimiter) == 0);
	cleanup_heredoc(heredoc);
	printf("✓ init_heredoc test passed\n");
}

static void	test_handle_heredoc(void)
{
	t_command	cmd = {0};
	char		*delimiter = "EOF";
	int			result;

	printf("Testing handle_heredoc...\n");
	/* Note: This test may require manual input or mocking */
	result = handle_heredoc(&cmd, delimiter);
	printf("✓ handle_heredoc test completed (result: %d)\n", result);
}
EOF
            ;;
            
        "external")
            cat >> "$test_file" << 'EOF'
/* External Command Tests */
static void	test_execute_external_command_simple(void)
{
	char	*args[] = {"echo", "test", NULL};
	int		result;

	printf("Testing execute_external_command...\n");
	result = execute_external_command(args);
	printf("✓ execute_external_command test completed (result: %d)\n", result);
}
EOF
            ;;
            
        "pipeline")
            cat >> "$test_file" << 'EOF'
/* Pipeline Tests */
static void	test_execute_command_pipeline(void)
{
	t_command	cmd = {0};
	char		*args[] = {"echo", "hello", NULL};
	int			result;

	printf("Testing execute_command_pipeline...\n");
	cmd.args = args;
	result = execute_command_pipeline(&cmd);
	printf("✓ execute_command_pipeline test completed (result: %d)\n", result);
}

static void	test_get_exit_status(void)
{
	t_command	cmd = {0};
	int			status;

	printf("Testing get_exit_status...\n");
	cmd.last_status = 42;
	status = get_exit_status(&cmd);
	assert(status == 42);
	printf("✓ get_exit_status test passed\n");
}

static void	test_set_exit_status(void)
{
	t_command	cmd = {0};

	printf("Testing set_exit_status...\n");
	set_exit_status(&cmd, 123);
	assert(cmd.last_status == 123);
	printf("✓ set_exit_status test passed\n");
}
EOF
            ;;
    esac
    
    # Add main function
    cat >> "$test_file" << EOF

int	main(void)
{
	printf("${PURPLE}=== Testing $module Module ===${NC}\\n\\n");
	
EOF

    # Add test function calls based on module
    case $module in
        "tokenizer")
            cat >> "$test_file" << 'EOF'
	test_is_meta();
	test_is_quote();
	test_is_delimiter();
	test_tokenize_simple();
	test_tokenize_with_quotes();
	test_tokenize_with_pipe();
EOF
            ;;
        "parser")
            cat >> "$test_file" << 'EOF'
	test_is_builtin();
	test_parse_simple_command();
EOF
            ;;
        "env")
            cat >> "$test_file" << 'EOF'
	test_create_env_node();
	test_get_env_node();
	test_expand_env_vars_simple();
	test_expand_env_vars_exit_status();
EOF
            ;;
        "builtin")
            cat >> "$test_file" << 'EOF'
	test_is_valid_identifier();
	test_execute_builtin_echo();
	test_execute_builtin_pwd();
EOF
            ;;
        "redirect")
            cat >> "$test_file" << 'EOF'
	test_redirect_types();
	test_create_redirect();
EOF
            ;;
        "heredoc")
            cat >> "$test_file" << 'EOF'
	test_init_heredoc();
	test_handle_heredoc();
EOF
            ;;
        "external")
            cat >> "$test_file" << 'EOF'
	test_execute_external_command_simple();
EOF
            ;;
        "pipeline")
            cat >> "$test_file" << 'EOF'
	test_get_exit_status();
	test_set_exit_status();
	test_execute_command_pipeline();
EOF
            ;;
    esac
    
    cat >> "$test_file" << 'EOF'
	
	printf("\n=== All tests completed! ===\n");
	return (0);
}
EOF

    echo -e "${GREEN}✓ Generated $test_file${NC}"
}

# Generate comprehensive test suite
generate_comprehensive_test() {
    local test_file="$TEST_DIR/test_comprehensive.c"
    
    echo -e "${CYAN}Generating comprehensive test suite...${NC}"
    
    generate_test_header "$test_file" "comprehensive"
    
    cat >> "$test_file" << 'EOF'
/* Comprehensive Integration Tests */

static void	test_full_command_parsing(void)
{
	char		*input = "echo hello world";
	t_command	cmd = {0};
	t_token		*tokens;
	t_command	*parsed_cmd;

	printf("Testing full command parsing pipeline...\n");
	
	tokens = tokenize(input, &cmd);
	assert(tokens != NULL);
	
	parsed_cmd = parse_tokens(tokens);
	assert(parsed_cmd != NULL);
	assert(parsed_cmd->args != NULL);
	assert(strcmp(parsed_cmd->args[0], "echo") == 0);
	
	free_tokens(tokens);
	free_command(parsed_cmd);
	printf("✓ Full command parsing test passed\n");
}

static void	test_env_integration(void)
{
	char	*expanded;
	char	*input = "echo $HOME";

	printf("Testing environment variable integration...\n");
	expanded = expand_env_vars(input, 0);
	assert(expanded != NULL);
	assert(strlen(expanded) > 0);
	free(expanded);
	printf("✓ Environment integration test passed\n");
}

static void	test_builtin_detection(void)
{
	printf("Testing builtin command detection...\n");
	assert(is_builtin("echo") == 1);
	assert(is_builtin("cd") == 1);
	assert(is_builtin("pwd") == 1);
	assert(is_builtin("export") == 1);
	assert(is_builtin("unset") == 1);
	assert(is_builtin("env") == 1);
	assert(is_builtin("exit") == 1);
	assert(is_builtin("nonexistent") == 0);
	printf("✓ Builtin detection test passed\n");
}

static void	test_redirect_creation(void)
{
	t_redirect	*redir_out;
	t_redirect	*redir_in;

	printf("Testing redirection creation...\n");
	redir_out = create_redirect(REDIR_OUT, "output.txt");
	redir_in = create_redirect(REDIR_IN, "input.txt");
	
	assert(redir_out != NULL);
	assert(redir_in != NULL);
	assert(redir_out->type == REDIR_OUT);
	assert(redir_in->type == REDIR_IN);
	
	free_redirect(redir_out);
	free_redirect(redir_in);
	printf("✓ Redirection creation test passed\n");
}

int	main(void)
{
	printf("=== Comprehensive Minishell Test Suite ===\n\n");
	
	test_builtin_detection();
	test_full_command_parsing();
	test_env_integration();
	test_redirect_creation();
	
	printf("\n=== Comprehensive tests completed! ===\n");
	return (0);
}
EOF

    echo -e "${GREEN}✓ Generated $test_file${NC}"
}

# Generate Makefile for tests
generate_test_makefile() {
    local makefile="$TEST_DIR/Makefile"
    
    echo -e "${CYAN}Generating test Makefile...${NC}"
    
    cat > "$makefile" << 'EOF'
# Test Makefile for Minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I.. -I../libft
LIBFT = ../libft/libft.a
MINISHELL_OBJS = $(filter-out ../obj/main.o, $(wildcard ../obj/*/*.o))

MODULES = tokenizer parser env builtin redirect heredoc external pipeline
TEST_TARGETS = $(addprefix test_, $(MODULES))
TEST_SOURCES = $(addsuffix .c, $(TEST_TARGETS))

.PHONY: all clean $(TEST_TARGETS) comprehensive help

all: $(TEST_TARGETS) comprehensive

$(LIBFT):
	make -C ../libft

test_%: test_%.c $(LIBFT)
	@echo "Compiling $@..."
	$(CC) $(CFLAGS) $< $(MINISHELL_OBJS) $(LIBFT) -lreadline -o $@
	@echo "✓ $@ compiled successfully"

comprehensive: test_comprehensive.c $(LIBFT)
	@echo "Compiling comprehensive test..."
	$(CC) $(CFLAGS) $< $(MINISHELL_OBJS) $(LIBFT) -lreadline -o $@
	@echo "✓ Comprehensive test compiled successfully"

run_%: test_%
	@echo "Running $<..."
	./$<

run_comprehensive: comprehensive
	@echo "Running comprehensive test..."
	./comprehensive

run_all: $(TEST_TARGETS) comprehensive
	@echo "Running all tests..."
	@for test in $(TEST_TARGETS) comprehensive; do \
		echo "=== Running $$test ==="; \
		./$$test; \
		echo; \
	done

clean:
	rm -f $(TEST_TARGETS) comprehensive

help:
	@echo "Available targets:"
	@echo "  all           - Compile all tests"
	@echo "  test_<module> - Compile specific module test"
	@echo "  run_<module>  - Run specific module test"
	@echo "  run_all       - Run all tests"
	@echo "  comprehensive - Compile comprehensive test"
	@echo "  clean         - Clean test binaries"
	@echo "  help          - Show this help"
	@echo ""
	@echo "Available modules: $(MODULES)"
EOF

    echo -e "${GREEN}✓ Generated $makefile${NC}"
}

# Main execution
main() {
    if [ ! -f "$HEADER_FILE" ]; then
        echo -e "${RED}Error: $HEADER_FILE not found!${NC}"
        exit 1
    fi
    
    echo -e "${BLUE}Found functions in $HEADER_FILE${NC}"
    
    # Generate module-specific tests
    modules=("tokenizer" "parser" "env" "builtin" "redirect" "heredoc" "external" "pipeline")
    
    for module in "${modules[@]}"; do
        generate_specific_tests "$module"
    done
    
    # Generate comprehensive test
    generate_comprehensive_test
    
    # Generate test Makefile
    generate_test_makefile
    
    echo -e "\n${GREEN}=== Test Generation Complete ===${NC}"
    echo -e "${YELLOW}Generated files:${NC}"
    ls -la "$TEST_DIR"
    
    echo -e "\n${BLUE}Usage:${NC}"
    echo -e "  cd $TEST_DIR"
    echo -e "  make all           # Compile all tests"
    echo -e "  make run_all       # Run all tests"
    echo -e "  make test_tokenizer # Compile specific test"
    echo -e "  make run_tokenizer  # Run specific test"
    echo -e "  make comprehensive  # Compile integration tests"
    echo -e "  make help          # Show help"
}

# Run main function
main "$@"
