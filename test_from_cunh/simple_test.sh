#!/bin/bash

# heredocの正しいテスト

cd "/home/chinachu/42/minishell_42"

echo "=== Testing Heredoc with Proper Format ==="

echo "1. Testing with bash:"
cat << EOF
This is heredoc content
Line 2
Line 3
EOF

echo ""
echo "2. Testing with minishell:"
printf "cat << EOF\nThis is heredoc content\nLine 2\nLine 3\nEOF\n" | ./minishell

echo ""
echo "3. Testing with cat -e (bash):"
cat << EOF | cat -e
This is heredoc content
Line 2
Line 3
EOF

echo ""
echo "4. Testing with cat -e (minishell):"
printf "cat << EOF | cat -e\nThis is heredoc content\nLine 2\nLine 3\nEOF\n" | ./minishell

echo ""
echo "5. Testing the problematic format (bash):"
printf "<< end cat -e\nsimple\ntest\nend\n" | bash

echo ""
echo "6. Testing the problematic format (minishell):"
printf "<< end cat -e\nsimple\ntest\nend\n" | ./minishell
