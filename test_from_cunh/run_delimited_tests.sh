#!/bin/bash

# 区切り文字付きテストコマンドを実行するスクリプト

cd "/home/chinachu/42/minishell_42"

if [ ! -f "./minishell" ]; then
    echo "Error: minishell not found. Please run 'make' first."
    exit 1
fi

test_count=0
pass_count=0
fail_count=0
timeout_count=0

echo "=== Running Extracted Test Commands (with delimiters) ==="
echo ""

run_delimited_test() {
    local cmd="$1"
    test_count=$((test_count + 1))
    echo "=== Test $test_count ==="
    echo "Command:"
    # 複数行コマンドを視覚的に表示
    echo "$cmd" | sed 's/^/  /'
    echo "Output:"
    
    # コマンドをminishellに送信
    printf "%s\n" "$cmd" | timeout 5 ./minishell
    
    local exit_code=$?
    echo "Exit code: $exit_code"
    
    if [ $exit_code -eq 0 ] || [ $exit_code -eq 1 ]; then
        pass_count=$((pass_count + 1))
        echo "Result: PASS"
    elif [ $exit_code -eq 124 ]; then
        timeout_count=$((timeout_count + 1))
        echo "Result: TIMEOUT"
    else
        fail_count=$((fail_count + 1))
        echo "Result: FAIL (exit code: $exit_code)"
    fi
    echo ""
}

# extracted_test_commands.txtから区切り文字を使ってテストを読み込み
input_file="extracted_test_commands.txt"
current_test=""
in_test=false

while IFS= read -r line; do
    if [[ "$line" == "--- TEST_START ---" ]]; then
        in_test=true
        current_test=""
    elif [[ "$line" == "--- TEST_END ---" ]]; then
        if [ "$in_test" = true ] && [ -n "$current_test" ]; then
            # 末尾の改行を除去
            current_test=${current_test%$'\n'}
            run_delimited_test "$current_test"
        fi
        in_test=false
        current_test=""
    elif [ "$in_test" = true ]; then
        # テストコマンドの一部として追加
        if [ -z "$current_test" ]; then
            current_test="$line"
        else
            current_test="$current_test"$'\n'"$line"
        fi
    fi
done < "$input_file"

echo "=== Final Summary ==="
echo "Total tests: $test_count"
echo "Passed: $pass_count"
echo "Failed: $fail_count"
echo "Timeouts: $timeout_count"

if [ $test_count -gt 0 ]; then
    success_rate=$(( (pass_count * 100) / test_count ))
    echo "Success rate: $success_rate%"
fi

if [ $timeout_count -gt 0 ]; then
    echo "⚠️  Warning: $timeout_count tests timed out (may indicate hanging)"
fi
