#!/bin/bash
# filepath: /home/chinachu/42/minishell_42/generate_compile_commands.sh

echo "Generating compile_commands.json..."

cat > compile_commands.json << 'EOF'
[
EOF

# すべての.cファイルを検索してエントリを作成
first=true
find srcs -name "*.c" | while read -r file; do
    if [ "$first" = true ]; then
        first=false
    else
        echo "," >> compile_commands.json
    fi
    
    cat >> compile_commands.json << EOF
  {
    "directory": "/home/chinachu/42/minishell_42",
    "command": "cc -Wall -Wextra -Werror -std=c89 -I. -I./libft -I./srcs -I./srcs/utils -I./srcs/tokenizer -I./srcs/parser -I./srcs/executor -I./srcs/redirect -I./srcs/builtin -c $file",
    "file": "$file"
  }
EOF
done

echo "" >> compile_commands.json
echo "]" >> compile_commands.json

echo "compile_commands.json generated successfully!"