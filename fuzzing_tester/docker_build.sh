#!/bin/bash

# Dockerイメージビルドスクリプト

set -e  # エラー時に終了

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

echo "=== Minishell Fuzzing Tester Docker Build ==="
echo "プロジェクトルート: ${PROJECT_ROOT}"
echo "スクリプトディレクトリ: ${SCRIPT_DIR}"

# Dockerfileが存在するかチェック
if [ ! -f "${SCRIPT_DIR}/Dockerfile" ]; then
    echo "エラー: Dockerfileが見つかりません: ${SCRIPT_DIR}/Dockerfile"
    exit 1
fi

# minishell.hが存在するかチェック
if [ ! -f "${PROJECT_ROOT}/minishell.h" ]; then
    echo "エラー: minishell.hが見つかりません: ${PROJECT_ROOT}/minishell.h"
    echo "このスクリプトはminishellプロジェクトのルートディレクトリから実行してください"
    exit 1
fi

echo "Dockerイメージをビルド中..."
echo "イメージ名: minishell-fuzzer"

# Dockerイメージをビルド
cd "${PROJECT_ROOT}"
docker build -t minishell-fuzzer -f fuzzing_tester/Dockerfile .

if [ $? -eq 0 ]; then
    echo "✅ Dockerイメージのビルドが完了しました"
    echo "次のコマンドでテストを実行できます:"
    echo "  ./fuzzing_tester/docker_run.sh"
else
    echo "❌ Dockerイメージのビルドに失敗しました"
    exit 1
fi