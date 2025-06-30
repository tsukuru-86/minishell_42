#!/bin/bash

# Docker実行スクリプト

set -e  # エラー時に終了

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

echo "=== Minishell Fuzzing Tester 実行 ==="

# デフォルト設定
TEST_COUNT=1000
CONTAINER_NAME="minishell-fuzzer-run"
RESULTS_DIR="${PROJECT_ROOT}/fuzzing_results"

# コマンドライン引数の解析
while [[ $# -gt 0 ]]; do
    case $1 in
        -n|--num-tests)
            TEST_COUNT="$2"
            shift 2
            ;;
        -o|--output)
            RESULTS_DIR="$2"
            shift 2
            ;;
        -h|--help)
            echo "使用方法: $0 [オプション]"
            echo "オプション:"
            echo "  -n, --num-tests NUM    実行するテスト回数 (デフォルト: 1000)"
            echo "  -o, --output DIR       結果出力ディレクトリ (デフォルト: ./fuzzing_results)"
            echo "  -h, --help             このヘルプを表示"
            exit 0
            ;;
        *)
            echo "未知のオプション: $1"
            echo "使用方法: $0 -h でヘルプを表示"
            exit 1
            ;;
    esac
done

echo "テスト回数: ${TEST_COUNT}"
echo "結果出力先: ${RESULTS_DIR}"

# 結果ディレクトリを作成（適切な権限で）
mkdir -p "${RESULTS_DIR}"
chmod 777 "${RESULTS_DIR}"

# 既存のコンテナを停止・削除
if docker ps -a --format "table {{.Names}}" | grep -q "^${CONTAINER_NAME}$"; then
    echo "既存のコンテナを停止・削除中..."
    docker stop "${CONTAINER_NAME}" >/dev/null 2>&1 || true
    docker rm "${CONTAINER_NAME}" >/dev/null 2>&1 || true
fi

# Dockerイメージが存在するかチェック
if ! docker images --format "table {{.Repository}}" | grep -q "^minishell-fuzzer$"; then
    echo "❌ Dockerイメージ 'minishell-fuzzer' が見つかりません"
    echo "先に以下のコマンドでイメージをビルドしてください:"
    echo "  ./fuzzing_tester/docker_build.sh"
    exit 1
fi

echo "Dockerコンテナを起動中..."
echo "コンテナ名: ${CONTAINER_NAME}"

# Dockerコンテナを実行
docker run \
    --name "${CONTAINER_NAME}" \
    --rm \
    -v "${RESULTS_DIR}:/app/test_results" \
    --security-opt seccomp=unconfined \
    --cap-drop=ALL \
    --cap-add=SYS_PTRACE \
    minishell-fuzzer \
    python3 /app/fuzzing_tester/fuzzer.py "${TEST_COUNT}"

echo ""
echo "=== テスト完了 ==="
echo "結果ファイルは以下に保存されました:"
echo "  ${RESULTS_DIR}"
echo ""

# 結果ファイルの概要を表示
if [ -f "${RESULTS_DIR}/stats.json" ]; then
    echo "=== 統計情報 ==="
    python3 -c "
import json
import sys
try:
    with open('${RESULTS_DIR}/stats.json', 'r') as f:
        stats = json.load(f)
    print(f\"総テスト数: {stats.get('total_tests', 0)}\")
    print(f\"差異発見数: {stats.get('differences_found', 0)}\")
    print(f\"クラッシュ数: {stats.get('crashes_found', 0)}\")
    print(f\"タイムアウト数: {stats.get('timeouts', 0)}\")
    print(f\"開始時刻: {stats.get('start_time', 'N/A')}\")
    print(f\"終了時刻: {stats.get('end_time', 'N/A')}\")
except Exception as e:
    print(f\"統計情報の読み込みエラー: {e}\")
" || echo "統計情報の表示に失敗しました"
fi

# 発見された問題ファイルの一覧
echo ""
echo "=== 発見された問題 ==="
DIFF_FILES=$(find "${RESULTS_DIR}" -name "difference_*.json" 2>/dev/null | wc -l)
CRASH_FILES=$(find "${RESULTS_DIR}" -name "crash_*.json" 2>/dev/null | wc -l)

if [ "${DIFF_FILES}" -gt 0 ]; then
    echo "差異ファイル: ${DIFF_FILES}個"
    ls "${RESULTS_DIR}"/difference_*.json 2>/dev/null | head -5
    if [ "${DIFF_FILES}" -gt 5 ]; then
        echo "... (他 $((DIFF_FILES - 5))個)"
    fi
fi

if [ "${CRASH_FILES}" -gt 0 ]; then
    echo "クラッシュファイル: ${CRASH_FILES}個"
    ls "${RESULTS_DIR}"/crash_*.json 2>/dev/null | head -5
    if [ "${CRASH_FILES}" -gt 5 ]; then
        echo "... (他 $((CRASH_FILES - 5))個)"
    fi
fi

if [ "${DIFF_FILES}" -eq 0 ] && [ "${CRASH_FILES}" -eq 0 ]; then
    echo "問題は発見されませんでした ✅"
fi

echo ""
echo "分析を行うには、結果ファイルを確認してください:"
echo "  cat ${RESULTS_DIR}/stats.json"
echo "  ls ${RESULTS_DIR}/"