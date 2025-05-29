#!/bin/bash

# valgrindログ解析スクリプト
# 作成日時: $(date '+%Y-%m-%d %H:%M:%S')
# 制約: for文・do-while文禁止

# 色付き出力用の定数
readonly RED='\033[0;31m'
readonly GREEN='\033[0;32m'
readonly YELLOW='\033[1;33m'
readonly BLUE='\033[0;34m'
readonly CYAN='\033[0;36m'
readonly NC='\033[0m' # No Color

# グローバル定数
readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly LOGS_DIR="$SCRIPT_DIR/logs"

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

# 関数: セクションヘッダー出力
print_section() {
	echo -e "${CYAN}=== $1 ===${NC}"
}

# 関数: ログファイル検証
validate_log_file() {
	local log_file="$1"
	
	if [ ! -f "$log_file" ]; then
		print_error "ログファイルが見つかりません: $log_file"
		return 1
	fi
	
	if [ ! -s "$log_file" ]; then
		print_error "ログファイルが空です: $log_file"
		return 1
	fi
	
	# valgrindログかどうかチェック
	if ! grep -q "valgrind" "$log_file"; then
		print_error "有効なvalgrindログファイルではありません: $log_file"
		return 1
	fi
	
	return 0
}

# 関数: メモリリーク解析
analyze_memory_leaks() {
	local log_file="$1"
	
	print_section "メモリリーク解析"
	
	# 確実なメモリリーク
	local definitely_lost=$(grep "definitely lost:" "$log_file" | tail -1 | awk '{print $4, $5}')
	if [ -n "$definitely_lost" ] && [ "$definitely_lost" != "0 bytes" ]; then
		print_error "確実なメモリリーク: $definitely_lost"
	else
		print_success "確実なメモリリーク: なし"
	fi
	
	# 間接的なメモリリーク
	local indirectly_lost=$(grep "indirectly lost:" "$log_file" | tail -1 | awk '{print $4, $5}')
	if [ -n "$indirectly_lost" ] && [ "$indirectly_lost" != "0 bytes" ]; then
		print_warning "間接的なメモリリーク: $indirectly_lost"
	else
		print_success "間接的なメモリリーク: なし"
	fi
	
	# 可能性のあるメモリリーク
	local possibly_lost=$(grep "possibly lost:" "$log_file" | tail -1 | awk '{print $4, $5}')
	if [ -n "$possibly_lost" ] && [ "$possibly_lost" != "0 bytes" ]; then
		print_warning "可能性のあるメモリリーク: $possibly_lost"
	else
		print_success "可能性のあるメモリリーク: なし"
	fi
	
	# 到達可能なメモリ
	local still_reachable=$(grep "still reachable:" "$log_file" | tail -1 | awk '{print $4, $5}')
	if [ -n "$still_reachable" ]; then
		print_info "到達可能なメモリ: $still_reachable"
	fi
}

# 関数: エラーサマリー解析
analyze_error_summary() {
	local log_file="$1"
	
	print_section "エラーサマリー"
	
	local error_summary=$(grep "ERROR SUMMARY:" "$log_file" | tail -1)
	if [ -n "$error_summary" ]; then
		local error_count=$(echo "$error_summary" | awk '{print $4}')
		if [ "$error_count" -eq 0 ]; then
			print_success "エラー数: $error_count"
		else
			print_error "エラー数: $error_count"
		fi
		echo "$error_summary"
	fi
	
	# ヒープサマリー
	local heap_summary=$(grep "in use at exit:" "$log_file")
	if [ -n "$heap_summary" ]; then
		print_info "ヒープ使用状況:"
		echo "$heap_summary"
	fi
}

# 関数: リーク箇所の詳細解析
analyze_leak_details() {
	local log_file="$1"
	
	print_section "リーク箇所詳細"
	
	# definitely lostの詳細
	if grep -q "definitely lost" "$log_file"; then
		print_warning "確実なリーク箇所:"
		grep -A 10 "definitely lost" "$log_file" | head -15
		echo ""
	fi
	
	# 関数別リーク統計
	print_info "関数別メモリ使用統計（上位10件）:"
	grep -E "at 0x[0-9A-F]+:" "$log_file" | \
		grep -oE "\(in [^)]+\)" | \
		sort | uniq -c | sort -nr | head -10 || true
}

# 関数: 修正推奨箇所の特定
identify_fix_recommendations() {
	local log_file="$1"
	
	print_section "修正推奨箇所"
	
	# 高リスクパターンの検出
	local high_risk_patterns=(
		"tokenizer"
		"parser" 
		"heredoc"
		"builtin"
		"malloc"
		"strdup"
		"ft_split"
	)
	
	local pattern
	local found=0
	
	# 各パターンを while ループで処理（for文禁止のため）
	echo "${high_risk_patterns[@]}" | tr ' ' '\n' | while read -r pattern; do
		if [ -n "$pattern" ]; then
			local matches=$(grep -i "$pattern" "$log_file" | wc -l)
			if [ "$matches" -gt 0 ]; then
				print_warning "高リスクエリア検出: $pattern ($matches 箇所)"
				found=1
			fi
		fi
	done
	
	# メモリ関連エラーの検出
	if grep -q "Invalid read\|Invalid write\|Use of uninitialised value" "$log_file"; then
		print_error "メモリアクセスエラーが検出されました"
		grep -E "Invalid read|Invalid write|Use of uninitialised value" "$log_file" | head -5
	fi
}

# 関数: 使用方法表示
show_usage() {
	echo "使用方法: $0 [ログファイル]"
	echo ""
	echo "引数:"
	echo "  ログファイル    解析するvalgrindログファイルのパス"
	echo "                 指定しない場合、最新のログファイルを自動選択"
	echo ""
	echo "オプション:"
	echo "  -h, --help     このヘルプメッセージを表示"
	echo "  -l, --list     利用可能なログファイル一覧を表示"
	echo ""
	echo "例:"
	echo "  $0                              # 最新ログを解析"
	echo "  $0 logs/valgrind_test_20241201_120000.log  # 指定ログを解析"
	echo "  $0 --list                       # ログファイル一覧表示"
}

# 関数: ログファイル一覧表示
list_log_files() {
	print_info "利用可能なログファイル:"
	if [ -d "$LOGS_DIR" ]; then
		ls -la "$LOGS_DIR"/*.log 2>/dev/null | awk '{print $9, $5, $6, $7, $8}' || print_warning "ログファイルが見つかりません"
	else
		print_warning "ログディレクトリが見つかりません: $LOGS_DIR"
	fi
}

# 関数: 最新ログファイル取得
get_latest_log() {
	if [ -d "$LOGS_DIR" ]; then
		ls -t "$LOGS_DIR"/valgrind_*.log 2>/dev/null | head -1
	fi
}

# メイン実行部
main() {
	local log_file="$1"
	
	# 引数処理
	case "${1:-}" in
		-h|--help)
			show_usage
			exit 0
			;;
		-l|--list)
			list_log_files
			exit 0
			;;
		"")
			log_file=$(get_latest_log)
			if [ -z "$log_file" ]; then
				print_error "ログファイルが見つかりません。test_valgrind.sh を先に実行してください"
				exit 1
			fi
			print_info "最新のログファイルを解析します: $log_file"
			;;
		*)
			if [ ! -f "$1" ]; then
				print_error "指定されたファイルが見つかりません: $1"
				exit 1
			fi
			;;
	esac
	
	print_info "valgrindログ解析を開始します"
	echo "対象ファイル: $log_file"
	echo ""
	
	# ログファイル検証
	if ! validate_log_file "$log_file"; then
		exit 1
	fi
	
	# 各種解析実行
	analyze_memory_leaks "$log_file"
	echo ""
	analyze_error_summary "$log_file"
	echo ""
	analyze_leak_details "$log_file"
	echo ""
	identify_fix_recommendations "$log_file"
	
	print_info "解析完了"
}

# スクリプト実行
main "$@"