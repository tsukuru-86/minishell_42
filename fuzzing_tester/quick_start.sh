#!/bin/bash

# クイックスタートスクリプト

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=== Minishell Fuzzing Tester クイックスタート ==="
echo ""

# ステップ1: Dockerイメージのビルド
echo "ステップ1: Dockerイメージをビルド中..."
"${SCRIPT_DIR}/docker_build.sh"

echo ""
echo "ステップ2: 簡単なテストを実行中 (100回)..."

# ステップ2: 簡単なテスト実行
"${SCRIPT_DIR}/docker_run.sh" -n 100

echo ""
echo "ステップ3: 結果を分析中..."

# ステップ3: 結果分析
if [ -d "../fuzzing_results" ]; then
    python3 "${SCRIPT_DIR}/analyze_results.py" "../fuzzing_results"
else
    echo "結果ディレクトリが見つかりません"
fi

echo ""
echo "=== クイックスタート完了 ==="
echo ""
echo "次のステップ:"
echo "1. より多くのテストを実行: ./fuzzing_tester/docker_run.sh -n 10000"
echo "2. 結果の詳細分析: python3 ./fuzzing_tester/analyze_results.py ./fuzzing_results"
echo "3. 特定の問題ファイルの確認: cat ./fuzzing_results/difference_*.json"