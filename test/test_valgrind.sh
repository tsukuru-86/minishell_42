#!/bin/bash

# minishell valgrind自動テストスクリプト
# 作成日時: $(date '+%Y-%m-%d %H:%M:%S')
# 制約: for文・do-while文禁止

# 色付き出力用の定数
readonly RED='\033[0;31m'
readonly GREEN='\033[0;32m'
readonly YELLOW='\033[1;33m'
readonly BLUE='\033[0;34m'
readonly NC='\033[0m' # No Color

# グローバル定数
readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly MINISHELL_PATH="$SCRIPT_DIR/minishell"
readonly TEST_INPUTS_DIR="$SCRIPT_DIR/test_inputs"
readonly LOGS_DIR="$SCRIPT_DIR/logs"
readonly TIMESTAMP="$(date '+%Y%m%d_%H%M%S')"
readonly LOG_FILE="$LOGS_DIR/valgrind_test_$TIMESTAMP.log"

# 関数: エラーメッセージ出力
print_error() {
	echo -e "${RED}[ERROR]${NC} $1" >&2
}

# 関数: 成功メッセージ出力
print_success() {
	echo -e "${GREEN}[SUCCESS]${NC} $1"
}

# 関数: 警告メッセージ出力
print_warning() {
	echo -e "${YELLOW}[WARNING]${NC} $1"
}

# 関数: 情報メッセージ出力
print_info() {
	echo -e "${BLUE}[INFO]${NC} $1"
}

# 関数: 環境チェック
check_environment() {
	print_info "環境チェックを開始しています..."
	
	# valgrindの存在確認
	if ! command -v valgrind >/dev/null 2>&1; then
		print_error "valgrindがインストールされていません"
		exit 1
	fi
	
	# minishellバイナリの存在確認
	if [ ! -f "$MINISHELL_PATH" ]; then
		print_warning "minishellバイナリが見つかりません。makeを実行します..."
		make -C "$SCRIPT_DIR" 2>/dev/null
		if [ ! -f "$MINISHELL_PATH" ]; then
			print_error "minishellのビルドに失敗しました"
			exit 1
		fi
	fi
	
	# テスト入力ディレクトリの確認
	if [ ! -d "$TEST_INPUTS_DIR" ]; then
		print_error "テスト入力ディレクトリが見つかりません: $TEST_INPUTS_DIR"
		exit 1
	fi
	
	# ログディレクトリの作成
	mkdir -p "$LOGS_DIR"
	
	print_success "環境チェック完了"
}

# 関数: valgrind実行
run_valgrind_test() {
	local test_name="$1"
	local input_file="$2"
	local test_log="$LOGS_DIR/valgrind_${test_name}_$TIMESTAMP.log"
	
	print_info "テスト実行中: $test_name"
	
	if [ ! -f "$input_file" ]; then
		print_error "テストファイルが見つかりません: $input_file"
		return 1
	fi
	
	# valgrind設定
	local valgrind_opts="--leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=$test_log"
	
	# テスト実行
	valgrind $valgrind_opts "$MINISHELL_PATH" < "$input_file" >/dev/null 2>&1
	local exit_code=$?
	
	# 結果確認
	if grep -q "ERROR SUMMARY: 0 errors" "$test_log" && grep -q "definitely lost: 0 bytes" "$test_log"; then
		print_success "$test_name - メモリリークなし"
		echo "SUCCESS: $test_name" >> "$LOG_FILE"
	else
		print_warning "$test_name - メモリリーク検出"
		echo "LEAK_DETECTED: $test_name" >> "$LOG_FILE"
	fi
	
	return $exit_code
}

# 関数: テスト結果サマリー
print_test_summary() {
	print_info "テスト結果サマリー"
	echo "================================"
	
	local total_tests=0
	local success_tests=0
	local leak_tests=0
	
	if [ -f "$LOG_FILE" ]; then
		total_tests=$(wc -l < "$LOG_FILE")
		success_tests=$(grep -c "SUCCESS:" "$LOG_FILE" 2>/dev/null || echo 0)
		leak_tests=$(grep -c "LEAK_DETECTED:" "$LOG_FILE" 2>/dev/null || echo 0)
	fi
	
	echo "総テスト数: $total_tests"
	echo "成功: $success_tests"
	echo "メモリリーク検出: $leak_tests"
	echo "================================"
	
	if [ "$leak_tests" -gt 0 ]; then
		print_warning "メモリリークが検出されました。詳細はログファイルを確認してください："
		echo "メインログ: $LOG_FILE"
		echo "詳細ログ: $LOGS_DIR/valgrind_*_$TIMESTAMP.log"
	else
		print_success "全てのテストでメモリリークは検出されませんでした"
	fi
}

# 関数: 使用方法表示
show_usage() {
	echo "使用方法: $0 [オプション]"
	echo "オプション:"
	echo "  -h, --help     このヘルプメッセージを表示"
	echo "  -v, --verbose  詳細出力モード"
	echo ""
	echo "このスクリプトは以下のテストを実行します:"
	echo "1. 基本的なビルトインコマンドテスト"
	echo "2. トークナイザーテスト（クォート処理など）"
	echo "3. パーサーテスト（引数拡張など）"
	echo "4. ヒアドキュメントテスト"
	echo "5. エラーケーステスト"
}

# メイン実行部
main() {
	# 引数処理
	case "${1:-}" in
		-h|--help)
			show_usage
			exit 0
			;;
		-v|--verbose)
			set -x
			;;
	esac
	
	print_info "minishell valgrindテストを開始します"
	echo "ログファイル: $LOG_FILE"
	echo "タイムスタンプ: $TIMESTAMP"
	echo ""
	
	# 環境チェック
	check_environment
	
	# テスト初期化
	echo "# minishell valgrind テスト結果 - $TIMESTAMP" > "$LOG_FILE"
	
	# 各テスト実行
	run_valgrind_test "basic" "$TEST_INPUTS_DIR/basic_tests.txt"
	run_valgrind_test "tokenizer" "$TEST_INPUTS_DIR/tokenizer_tests.txt"
	run_valgrind_test "parser" "$TEST_INPUTS_DIR/parser_tests.txt"
	run_valgrind_test "heredoc" "$TEST_INPUTS_DIR/heredoc_tests.txt"
	run_valgrind_test "error" "$TEST_INPUTS_DIR/error_tests.txt"
	
	# 結果サマリー表示
	print_test_summary
	
	print_info "テスト完了。ログ解析には analyze_valgrind_log.sh を使用してください"
}

# スクリプト実行
main "$@"