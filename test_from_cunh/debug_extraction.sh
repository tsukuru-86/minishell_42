#!/bin/bash

# デバッグ用のシンプルな抽出スクリプト

input_file="/home/chinachu/42/minishell_42/minishell_tester-nda-cunh/main.vala"

echo "Testing command extraction..."

# heredocコマンドを含む特定の行をテスト
test_line='	add_test.begin({"<< end cat -e \nsimple\ntest\nend"});'

echo "Input line: $test_line"

# コマンド部分を抽出
cmd=$(echo "$test_line" | sed 's/.*add_test\.begin({\(.*\)}).*/\1/')
echo "Extracted cmd: $cmd"

# クォートを削除
if [[ "$cmd" =~ ^\"(.*)\"$ ]]; then
    extracted="${BASH_REMATCH[1]}"
    echo "Extracted content: $extracted"
    
    # \nを改行に変換してテスト
    echo "Converting \\n to newlines:"
    printf "%b\n" "$extracted"
    echo "--- End of conversion ---"
fi
