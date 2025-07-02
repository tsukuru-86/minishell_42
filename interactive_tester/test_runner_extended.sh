#!/bin/bash

# minishell 拡張インタラクティブテスト実行スクリプト
# test1とtest2の主要テストケースを統合
# 作成日: 2025年6月24日

# カラー出力用の定義
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# ログファイル名
LOG_FILE="test_runner_extended_$(date +%Y%m%d_%H%M%S).log"

# ヘルプ表示関数
show_help() {
	echo -e "${CYAN}=== minishell 拡張インタラクティブテストランナー ===${NC}"
	echo ""
	echo "使用方法: $0 [オプション]"
	echo ""
	echo "オプション:"
	echo "  -h, --help         このヘルプを表示"
	echo "  -v, --verbose      詳細な出力を表示"
	echo "  -q, --quiet        エラー以外の出力を抑制"
	echo "  -c, --clean        テスト前にminishellを再ビルド"
	echo "  -t, --timeout      タイムアウト時間を設定（デフォルト: 10秒）"
	echo "  -s, --summary      サマリーのみ表示"
	echo "  --syntax-only      構文エラーテストのみ実行"
	echo "  --builtin-only     ビルトインテストのみ実行"
	echo "  --pipe-only        パイプテストのみ実行"
	echo "  --redirect-only    リダイレクトテストのみ実行"
	echo "  --variable-only    変数展開テストのみ実行"
	echo ""
	echo "例:"
	echo "  $0                     # 全テスト実行"
	echo "  $0 -v                  # 詳細出力で実行"
	echo "  $0 -c                  # クリーンビルド後に実行"
	echo "  $0 -t 15               # タイムアウトを15秒に設定"
	echo "  $0 --syntax-only       # 構文エラーテストのみ"
	echo "  $0 --builtin-only -v   # ビルトインテストを詳細表示"
	echo ""
	echo "統合テストケース:"
	echo "  • 構文エラーテスト: 20ケース"
	echo "  • ビルトインテスト: 30ケース"
	echo "  • パイプラインテスト: 15ケース" 
	echo "  • リダイレクトテスト: 25ケース"
	echo "  • 変数展開テスト: 20ケース"
	echo "  • 履歴・エラーハンドリング: 10ケース"
	echo "  ${GREEN}合計: 約120テストケース${NC}"
	echo ""
}

# エラー出力関数
error() {
	echo -e "${RED}エラー: $1${NC}" >&2
	echo "$(date): ERROR: $1" >> "$LOG_FILE"
}

# 成功出力関数
success() {
	echo -e "${GREEN}✓ $1${NC}"
	echo "$(date): SUCCESS: $1" >> "$LOG_FILE"
}

# 情報出力関数
info() {
	if [[ "$QUIET" != "1" ]]; then
		echo -e "${BLUE}→ $1${NC}"
	fi
	echo "$(date): INFO: $1" >> "$LOG_FILE"
}

# 警告出力関数
warning() {
	echo -e "${YELLOW}⚠ $1${NC}"
	echo "$(date): WARNING: $1" >> "$LOG_FILE"
}

# 詳細出力関数
verbose() {
	if [[ "$VERBOSE" == "1" ]]; then
		echo -e "${CYAN}[詳細] $1${NC}"
	fi
	echo "$(date): VERBOSE: $1" >> "$LOG_FILE"
}

# カテゴリヘッダー出力関数
category_header() {
	if [[ "$QUIET" != "1" ]]; then
		echo -e "${PURPLE}◆◆◆ $1 ◆◆◆${NC}"
	fi
	echo "$(date): CATEGORY: $1" >> "$LOG_FILE"
}

# 環境チェック関数
check_environment() {
	info "環境チェック中..."
	
	# expectの存在確認
	if ! command -v expect >/dev/null 2>&1; then
		error "expectがインストールされていません"
		echo ""
		echo "インストール方法:"
		echo "  Ubuntu/Debian: sudo apt-get install expect"
		echo "  CentOS/RHEL:   sudo yum install expect"
		echo "  macOS:         brew install expect"
		echo ""
		return 1
	fi
	verbose "expect が見つかりました: $(which expect)"
	
	# Makefileの存在確認（上位ディレクトリも確認）
	if [[ ! -f "../Makefile" ]] && [[ ! -f "Makefile" ]]; then
		error "Makefile が見つかりません"
		info "minishellプロジェクトのルートディレクトリまたはinteractive_tester/ディレクトリで実行してください"
		return 1
	fi
	verbose "Makefile が見つかりました"
	
	# expectスクリプトの存在確認
	if [[ ! -f "interactive_test_extended.exp" ]]; then
		error "interactive_test_extended.exp が見つかりません"
		info "expectスクリプトを先に作成してください"
		return 1
	fi
	verbose "interactive_test_extended.exp が見つかりました"
	
	success "環境チェック完了"
	return 0
}

# minishellビルド関数
build_minishell() {
	info "minishell をビルド中..."
	
	# 上位ディレクトリに移動してビルド
	local original_dir=$(pwd)
	
	if [[ -f "../Makefile" ]]; then
		cd ..
		verbose "上位ディレクトリに移動してビルドします"
	fi
	
	if [[ "$CLEAN_BUILD" == "1" ]]; then
		verbose "クリーンビルドを実行します"
		if ! make fclean >/dev/null 2>&1; then
			warning "make fclean でエラーが発生しましたが続行します"
		fi
	fi
	
	# ビルド実行
	if [[ "$VERBOSE" == "1" ]]; then
		make
	else
		make >/dev/null 2>&1
	fi
	
	local build_result=$?
	cd "$original_dir"
	
	if [[ $build_result -ne 0 ]]; then
		error "minishell のビルドに失敗しました"
		info "make コマンドを手動で実行してエラーを確認してください"
		return 1
	fi
	
	# 実行ファイルの存在確認
	if [[ ! -f "../minishell" ]]; then
		error "minishell 実行ファイルが生成されませんでした"
		return 1
	fi
		
	# 実行権限の確認
	if [[ ! -x "../minishell" ]]; then
		warning "../minishell に実行権限がありません。権限を付与します"
		chmod +x "../minishell"
	fi
	
	success "../minishell のビルドが完了しました"
	return 0
}

# expectスクリプトの実行権限設定
setup_expect_script() {
	info "expectスクリプトの設定中..."
	
	if [[ ! -x "interactive_test_extended.exp" ]]; then
		verbose "interactive_test_extended.exp に実行権限を付与します"
		chmod +x "interactive_test_extended.exp"
	fi
	
	success "expectスクリプトの設定が完了しました"
	return 0
}

# テスト実行関数
run_tests() {
	info "拡張インタラクティブテストを開始します..."
	category_header "約120個のテストケースを実行中..."
	
	# タイムアウト設定を反映
	if [[ -n "$TIMEOUT" ]]; then
		verbose "タイムアウトを ${TIMEOUT} 秒に設定します"
		# expectスクリプト内のタイムアウト値を一時的に変更
		sed "s/set timeout 10/set timeout $TIMEOUT/" interactive_test_extended.exp > interactive_test_tmp.exp
		chmod +x interactive_test_tmp.exp
		EXPECT_SCRIPT="interactive_test_tmp.exp"
	else
		EXPECT_SCRIPT="interactive_test_extended.exp"
	fi
	
	# テスト実行
	echo ""
	local start_time=$(date +%s)
	
	if [[ "$VERBOSE" == "1" ]]; then
		./"$EXPECT_SCRIPT"
	elif [[ "$QUIET" == "1" ]]; then
		./"$EXPECT_SCRIPT" >/dev/null 2>&1
	elif [[ "$SUMMARY_ONLY" == "1" ]]; then
		./"$EXPECT_SCRIPT" | grep -E "(=== |◆◆◆|✓ |✗ |実行テスト数|成功率|カテゴリ別結果)"
	else
		./"$EXPECT_SCRIPT"
	fi
	
	local test_result=$?
	local end_time=$(date +%s)
	local duration=$((end_time - start_time))
	
	# 一時ファイルの削除
	if [[ -f "interactive_test_tmp.exp" ]]; then
		rm -f "interactive_test_tmp.exp"
	fi
	
	echo ""
	info "テスト実行時間: ${duration}秒"
	
	if [[ $test_result -eq 0 ]]; then
		success "すべてのテストが成功しました"
	else
		error "一部のテストが失敗しました"
		info "詳細な結果は生成されたテスト結果ファイルを確認してください"
	fi
	
	# 結果ファイルの場所を表示
	local result_files=$(ls test_results_extended_*.txt 2>/dev/null | tail -1)
	if [[ -n "$result_files" ]]; then
		info "詳細結果: $result_files"
	fi
	
	return $test_result
}

# クリーンアップ関数
cleanup() {
	verbose "クリーンアップ中..."
	
	# 一時ファイルの削除
	if [[ -f "interactive_test_tmp.exp" ]]; then
		rm -f "interactive_test_tmp.exp"
	fi
	
	# テスト中に作成された可能性のある一時ファイル
	if [[ -f "test_output.txt" ]]; then
		rm -f "test_output.txt"
		verbose "テスト用一時ファイルを削除しました"
	fi
	
	# minishellのコピーを削除
	if [[ -f "../minishell" ]] && [[ -f "../minishell" ]]; then
		rm -f "../minishell"
		verbose "minishellのコピーを削除しました"
	fi
}

# シグナルハンドラ設定
trap cleanup EXIT INT TERM

# メイン処理
main() {
	# デフォルト値
	VERBOSE=0
	QUIET=0
	CLEAN_BUILD=0
	TIMEOUT=""
	SUMMARY_ONLY=0
	CATEGORY_FILTER=""
	
	# コマンドライン引数の解析
	while [[ $# -gt 0 ]]; do
		case $1 in
			-h|--help)
				show_help
				exit 0
				;;
			-v|--verbose)
				VERBOSE=1
				shift
				;;
			-q|--quiet)
				QUIET=1
				shift
				;;
			-c|--clean)
				CLEAN_BUILD=1
				shift
				;;
			-s|--summary)
				SUMMARY_ONLY=1
				shift
				;;
			-t|--timeout)
				if [[ -n "$2" && "$2" =~ ^[0-9]+$ ]]; then
					TIMEOUT="$2"
					shift 2
				else
					error "無効なタイムアウト値: $2"
					exit 1
				fi
				;;
			--syntax-only)
				CATEGORY_FILTER="syntax"
				shift
				;;
			--builtin-only)
				CATEGORY_FILTER="builtin"
				shift
				;;
			--pipe-only)
				CATEGORY_FILTER="pipe"
				shift
				;;
			--redirect-only)
				CATEGORY_FILTER="redirect"
				shift
				;;
			--variable-only)
				CATEGORY_FILTER="variable"
				shift
				;;
			*)
				error "無効なオプション: $1"
				show_help
				exit 1
				;;
		esac
	done
	
	# QUIETとVERBOSEの競合チェック
	if [[ "$QUIET" == "1" && "$VERBOSE" == "1" ]]; then
		error "-q (quiet) と -v (verbose) オプションは同時に使用できません"
		exit 1
	fi
	
	echo -e "${CYAN}=== minishell 拡張インタラクティブテストランナー ===${NC}"
	echo -e "${PURPLE}統合テストケース: 約120個 (test1 + test2 + 追加機能)${NC}"
	echo "開始時刻: $(date)"
	echo "ログファイル: $LOG_FILE"
	if [[ -n "$CATEGORY_FILTER" ]]; then
		echo -e "${YELLOW}カテゴリフィルタ: $CATEGORY_FILTER${NC}"
	fi
	echo ""
	
	# ログファイル初期化
	echo "=== minishell 拡張インタラクティブテストランナー ログ ===" > "$LOG_FILE"
	echo "開始時刻: $(date)" >> "$LOG_FILE"
	echo "統合テストケース: 約120個" >> "$LOG_FILE"
	if [[ -n "$CATEGORY_FILTER" ]]; then
		echo "カテゴリフィルタ: $CATEGORY_FILTER" >> "$LOG_FILE"
	fi
	echo "" >> "$LOG_FILE"
	
	# 処理実行
	if ! check_environment; then
		exit 1
	fi
	
	if ! build_minishell; then
		exit 1
	fi
	
	if ! setup_expect_script; then
		exit 1
	fi
	
	if ! run_tests; then
		exit 1
	fi
	
	echo ""
	echo "完了時刻: $(date)"
	success "拡張テストランナーが正常に完了しました"
	
	return 0
}

# スクリプト実行
main "$@"