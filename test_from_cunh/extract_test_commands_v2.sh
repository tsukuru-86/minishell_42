#!/bin/bash

# main.valaからテストコマンドを抽出し、minishellで実行可能な形式にするスクリプト
# \n を実際の改行に変換して処理

input_file="/home/chinachu/42/minishell_42/minishell_tester-nda-cunh/main.vala"
output_text="/home/chinachu/42/minishell_42/extracted_test_commands.txt"
output_script="/home/chinachu/42/minishell_42/run_extracted_tests.sh"

echo "Extracting test commands from $input_file..."

# extracted_test_commands.txtに抽出結果を保存
echo "# テストコマンド一覧 (main.valaから抽出)" > "$output_text"
echo "# 各行が1つのテストコマンド（\\nは実際の改行に変換済み）" >> "$output_text"
echo "" >> "$output_text"

# run_extracted_tests.shスクリプトを生成
cat > "$output_script" << 'EOF'
#!/bin/bash

# main.valaから抽出されたテストコマンドを実行するスクリプト

cd "/home/chinachu/42/minishell_42"

if [ ! -f "./minishell" ]; then
    echo "Error: minishell not found. Please run 'make' first."
    exit 1
fi

test_count=0
pass_count=0
fail_count=0

echo "=== Running Extracted Test Commands ==="
echo ""

run_test() {
    local cmd="$1"
    test_count=$((test_count + 1))
    echo "=== Test $test_count ==="
    echo -n "Command: "
    # 改行文字を視覚的に表示
    echo "$cmd" | sed 's/$/\\n/' | tr -d '\n' | sed 's/\\n$//' | sed 's/\\n/\\n/g'
    echo ""
    echo "Output:"
    
    # コマンドをminishellに送信
    printf "%s\n" "$cmd" | timeout 10 ./minishell
    
    local exit_code=$?
    if [ $exit_code -eq 0 ] || [ $exit_code -eq 1 ]; then
        pass_count=$((pass_count + 1))
        echo "Result: PASS"
    else
        fail_count=$((fail_count + 1))
        echo "Result: FAIL (exit code: $exit_code)"
    fi
    echo ""
}

EOF

# 一時ファイルでコマンドリストを作成
temp_file=$(mktemp)
temp_script_part=$(mktemp)

# main.valaからadd_test.begin行を抽出
grep 'add_test\.begin(' "$input_file" | while read line; do
    # コメントアウトされた行をスキップ
    if [[ "$line" =~ ^[[:space:]]*// ]]; then
        continue
    fi
    
    # シンプルなsedでコマンド部分を抽出
    # {"command"} または {"""command"""} パターン
    if echo "$line" | grep -q 'add_test\.begin({'; then
        # {} の中身を抽出
        cmd=$(echo "$line" | sed 's/.*add_test\.begin({\(.*\)}).*/\1/')
        
        # "..." または """...""" の中身を抽出
        if [[ "$cmd" =~ ^\"\"\"(.*)\"\"\"$ ]]; then
            # トリプルクォートの場合
            extracted="${BASH_REMATCH[1]}"
        elif [[ "$cmd" =~ ^\"(.*)\"$ ]]; then
            # ダブルクォートの場合（貪欲マッチでコマンド全体を取得）
            extracted="${BASH_REMATCH[1]}"
        else
            continue
        fi
        
        # \n を実際の改行に変換して一時ファイルに保存
        printf "%b\n" "$extracted" >> "$temp_file"
        
        # 実行スクリプト用にエスケープされたコマンドを保存
        printf "run_test %q\n" "$extracted" >> "$temp_script_part"
    fi
done

# 一時ファイルの内容を最終ファイルに移動
if [ -f "$temp_file" ]; then
    cat "$temp_file" >> "$output_text"
    cat "$temp_script_part" >> "$output_script"
    rm "$temp_file" "$temp_script_part"
fi

# スクリプトの末尾を追加
cat >> "$output_script" << 'EOF'

echo "=== Test Results ==="
echo "Total tests: $test_count"
echo "Passed: $pass_count"
echo "Failed: $fail_count"
if [ $test_count -gt 0 ]; then
    score=$((pass_count * 100 / test_count))
    echo "Score: $score%"
fi
EOF

chmod +x "$output_script"

echo "Test commands extracted to: $output_text"
echo "Test script created: $output_script"
echo "Run tests with: $output_script"

# 抽出されたコマンド数を表示
command_count=$(grep -v '^#' "$output_text" | grep -v '^$' | wc -l)
echo "Total extracted commands: $command_count"
