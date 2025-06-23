#!/bin/bash

# main.valaからテストコマンドを抽出し、minishellで実行可能な形式にするスクリプト

input_file="/home/chinachu/42/minishell_42/minishell_tester-nda-cunh/main.vala"
output_text="/home/chinachu/42/minishell_42/extracted_test_commands.txt"
output_script="/home/chinachu/42/minishell_42/run_extracted_tests.sh"

echo "Extracting test commands from $input_file..."

# extracted_test_commands.txtに抽出結果を保存
echo "# テストコマンド一覧 (main.valaから抽出)" > "$output_text"
echo "# 各行が1つのテストコマンド" >> "$output_text"
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
    echo "Command: $cmd"
    echo "Output:"
    
    # 複数行コマンド（\nを含む）の場合はprintf -bを使用して改行を処理
    if echo "$cmd" | grep -q '\\n'; then
        printf "%b\n" "$cmd" | timeout 10 ./minishell
    else
        echo "$cmd" | timeout 10 ./minishell
    fi
    
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

# main.valaからadd_test.begin行を抽出し、各コマンドを処理
grep -n 'add_test\.begin(' "$input_file" | while IFS=: read -r line_num line_content; do
    # コメントアウトされた行をスキップ
    if [[ "$line_content" =~ ^[[:space:]]*// ]]; then
        continue
    fi
    
    # add_test.begin({...});のパターンからコマンドを抽出
    if [[ "$line_content" =~ add_test\.begin\(\{(.*)\}\) ]]; then
        # {} 内の内容を取得
        inner_content="${BASH_REMATCH[1]}"
        
        # """...""" パターンを処理
        while [[ "$inner_content" =~ \"\"\"([^\"]*) ]]; do
            cmd="${BASH_REMATCH[1]}"
            # 前後の空白を削除
            cmd=$(echo "$cmd" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
            
            if [ -n "$cmd" ]; then
                # \nを実際の改行に変換してファイルに保存
                printf "%b\n" "$cmd" >> "$output_text"
                # スクリプトにはエスケープされた形で保存
                echo "run_test $(printf "%q" "$cmd")" >> "$output_script"
            fi
            
            # 処理した部分を削除
            inner_content=$(echo "$inner_content" | sed 's/"""[^"]*"""[[:space:]]*,*[[:space:]]*//')
        done
        
        # "..." パターンを処理
        while [[ "$inner_content" =~ \"([^\"]*)\" ]]; do
            cmd="${BASH_REMATCH[1]}"
            # 前後の空白を削除
            cmd=$(echo "$cmd" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
            
            if [ -n "$cmd" ]; then
                # \nを実際の改行に変換してファイルに保存
                printf "%b\n" "$cmd" >> "$output_text"
                # スクリプトにはエスケープされた形で保存
                echo "run_test $(printf "%q" "$cmd")" >> "$output_script"
            fi
            
            # 処理した部分を削除
            inner_content=$(echo "$inner_content" | sed 's/"[^"]*"[[:space:]]*,*[[:space:]]*//')
        done
    fi
done

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
