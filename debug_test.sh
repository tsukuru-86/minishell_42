#!/bin/bash

echo "=== Debug test for pipe+redirect ==="

# まずminishellをビルド（デバッグ版）
make debug

echo "--- Test command: printf 'Syntax Error!' | > file_out ---"
echo "printf 'Syntax Error!' | > file_out" | strace -e write ./minishell 2>&1 | head -20

echo
echo "--- Test command: printf 'Syntax Error!' |> file_out ---"  
echo "printf 'Syntax Error!' |> file_out" | strace -e write ./minishell 2>&1 | head -20