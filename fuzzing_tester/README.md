# Minishell Fuzzing Tester

このテスターは、bashとminishellの動作を比較するためのfuzzingツールです。
ランダムに生成された入力文字列を両方のシェルに送信し、出力と終了ステータスを比較します。

## 特徴
- bashで入力可能なすべての文字を使用したランダムテスト
- 安全なDocker環境での実行
- 詳細な結果分析とレポート生成
- 危険なコマンドの自動フィルタリング
- リアルタイム進捗表示

## 必要な環境
- Docker
- Python 3.8+
- GNU/Linux環境（Ubuntu 22.04推奨）

## ファイル構成
- `fuzzer.py`: メインのfuzzingテスター（Python）
- `analyze_results.py`: 結果分析スクリプト
- `Dockerfile`: Docker環境の設定（Ubuntu 22.04ベース）
- `docker_build.sh`: Dockerイメージのビルドスクリプト
- `docker_run.sh`: Dockerコンテナの実行スクリプト
- `quick_start.sh`: 全自動セットアップ・実行スクリプト

## クイックスタート
最も簡単な方法：
```bash
./fuzzing_tester/quick_start.sh
```

## 手動での使用方法

### 1. Dockerイメージのビルド
```bash
./fuzzing_tester/docker_build.sh
```

### 2. テストの実行
```bash
# デフォルト（1000回のテスト）
./fuzzing_tester/docker_run.sh

# カスタム回数でのテスト
./fuzzing_tester/docker_run.sh -n 10000

# カスタム出力ディレクトリ
./fuzzing_tester/docker_run.sh -o /path/to/results
```

### 3. 結果の分析
```bash
python3 ./fuzzing_tester/analyze_results.py ./fuzzing_results
```

## オプション

### docker_run.sh のオプション
- `-n, --num-tests NUM`: テスト回数を指定（デフォルト: 1000）
- `-o, --output DIR`: 結果の出力ディレクトリを指定
- `-h, --help`: ヘルプを表示

### fuzzer.py の設定
- `max_input_length`: 最大入力長（デフォルト: 1000文字）
- `test_timeout`: タイムアウト時間（デフォルト: 5秒）
- `dangerous_commands`: フィルタリングする危険コマンドのリスト

## 出力ファイル
テスト実行後、以下のファイルが生成されます：

- `stats.json`: 全体の統計情報
- `difference_*.json`: bashとminishellで結果が異なったケース
- `crash_*.json`: クラッシュが発生したケース

## 結果の読み方

### 統計情報の例
```json
{
  "total_tests": 1000,
  "differences_found": 5,
  "crashes_found": 2,
  "timeouts": 1,
  "start_time": "2025-06-30T03:00:00",
  "end_time": "2025-06-30T03:05:00"
}
```

### 差異ファイルの例
```json
{
  "timestamp": "2025-06-30T03:01:23",
  "input": "'echo hello | cat'",
  "bash_result": {
    "stdout": "hello\\n",
    "stderr": "",
    "return_code": 0
  },
  "minishell_result": {
    "stdout": "hello",
    "stderr": "",
    "return_code": 0
  },
  "differences": ["標準出力が異なります"]
}
```

## 安全性について
- すべてのテストはDockerコンテナ内で実行されます
- 危険なコマンド（rm, format, shutdown等）は自動的にフィルタリングされます
- セキュリティ制約により、システムに害を与えるリスクを最小化
- コンテナは実行後に自動的に削除されます

## トラブルシューティング

### Dockerイメージのビルドに失敗する場合
```bash
# Dockerの権限確認
docker --version
sudo docker --version

# キャッシュをクリアしてリビルド
docker system prune -f
./fuzzing_tester/docker_build.sh
```

### メモリ不足のエラーが発生する場合
```bash
# より少ないテスト回数で実行
./fuzzing_tester/docker_run.sh -n 100
```

### 権限エラーが発生する場合
```bash
# 実行権限を再設定
chmod +x fuzzing_tester/*.sh fuzzing_tester/*.py
```

## 高度な使用方法

### カスタム文字セットでのテスト
`fuzzer.py`内の文字セット定義を変更することで、特定の文字に焦点を当てたテストが可能です。

### 継続的なテスト
```bash
# 長時間のテスト（10万回）
./fuzzing_tester/docker_run.sh -n 100000
```

### 結果の詳細分析
```bash
# 特定の差異ファイルの詳細確認
cat fuzzing_results/difference_0.json | python3 -m json.tool

# クラッシュファイルの一覧
ls fuzzing_results/crash_*.json
```

## 貢献とフィードバック
このツールの改善案やバグ報告は歓迎します。特に以下の点については継続的な改善を行っています：

- テストケースの生成戦略
- 結果分析アルゴリズム
- パフォーマンスの最適化
- 安全性の向上