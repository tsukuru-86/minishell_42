# Minishell 危険度テスター（Danger Tester）

このテスターは、**minishellの頑健性を検証**するための特化ツールです。
できるだけハイリスクな危険なコマンドを実行して、minishellがそれを安全にエラーとして処理し、クリーンに終了することを確認します。

## 目的

**minishellの堅牢性テスト**を行い、以下のイレギュラーな事態に対する耐性を確認：
- OSやminishell自体のクラッシュ
- メモリリーク・メモリエラー（SEGV）
- スタックオーバーラン
- 無限ループ・デッドロック
- フリーズ・ハング
- その他のあらゆる予期しない動作

## 特徴

- **積極的危険コマンド実行**: fork bomb、無限ループ、大量データ生成などを含む
- **リアルタイム進捗表示**: 個別テスト番号と詳細ステータス表示
- **固定60秒タイムアウト**: 長時間実行コマンドの適切な制御
- **Valgrind統合**: メモリエラーとリークの自動検出
- **安全なDocker環境**: ホストシステムへの影響を最小化
- **詳細な結果分析**: クラッシュパターンとメモリ問題の分類

## 必要な環境
- Docker
- Python 3.8+
- GNU/Linux環境（Ubuntu 22.04推奨）

## ファイル構成
- `fuzzer.py`: メインの危険度テスター（Python）
- `analyze_results.py`: 結果分析スクリプト
- `Dockerfile`: Docker環境の設定（Ubuntu 22.04ベース、Valgrind含む）
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

### 2. 危険度テストの実行
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
- `test_timeout`: 固定60秒タイムアウト
- `use_valgrind`: Valgrindメモリ検査の有効/無効（デフォルト: 有効）
- `dangerous_commands`: 積極的にテストする危険コマンドリスト

## 出力ファイル
テスト実行後、以下のファイルが生成されます：

- `stats.json`: 全体の統計情報
- `difference_*.json`: bashとminishellで結果が異なったケース
- `crash_*.json`: クラッシュが発生したケース
- `memory_*.json`: **Valgrindで検出されたメモリ問題**

## 結果の読み方

### 統計情報の例
```json
{
  "total_tests": 1000,
  "differences_found": 45,
  "crashes_found": 12,
  "bash_crashes": 2,
  "minishell_crashes": 10,
  "timeouts": 8,
  "memory_errors_found": 3,
  "memory_leaks_found": 7,
  "dangerous_commands_tested": 156,
  "start_time": "2025-01-01T00:00:00",
  "end_time": "2025-01-01T01:30:00"
}
```

### 進捗表示の例
```
[テスト #0001] OK | 入力長:  45 | 実行時間:0.12s
[テスト #0002] MINI_CRASH | 入力長: 234 | 実行時間:2.34s
                    入力: 'while true; do echo hello; done'
[テスト #0003] MEM_LEAK | 入力長: 123 | 実行時間:5.67s
                    入力: 'cat /dev/urandom | head -n 1000'
[テスト #0004] DIFF | TIMEOUT | 入力長:1024 | 実行時間:60.00s
                    入力: 'yes | head -n 999999999'
```

### 差異ファイルの例
```json
{
  "test_number": 42,
  "timestamp": "2025-01-01T00:05:23",
  "input": "':(){ :|:& };:'",
  "input_length": 13,
  "bash_result": {
    "stdout": "",
    "stderr": "bash: fork: Resource temporarily unavailable",
    "return_code": 1,
    "crashed": false
  },
  "minishell_result": {
    "stdout": "",
    "stderr": "",
    "return_code": -11,
    "crashed": true,
    "memory_error": true
  },
  "differences": ["終了ステータス: bash=1, minishell=-11", "クラッシュ状態: bash=False, minishell=True", "Valgrindメモリエラー検出"]
}
```

## 危険度テストについて

### テスト対象コマンド
本テスターは以下のような高リスクコマンドを積極的に実行します：

1. **無限ループ系**
   - `while true; do echo hello; done`
   - `yes` (without pipe)
   - `seq 1 1000000`

2. **Fork bomb系**
   - `:(){ :|:& };:`
   - 深いサブシェル呼び出し

3. **大量データ生成系**
   - `cat /dev/urandom`
   - `dd if=/dev/zero of=/dev/null`
   - `find / -name '*'`

4. **複雑構文系**
   - 深いパイプチェーン（20段以上）
   - 大量リダイレクション
   - 複雑な変数展開とコマンド置換

5. **境界値系**
   - ARG_MAX制限周辺の超長入力
   - 特殊文字の大量使用

### 実行順序
1. **危険度テスト**（メイン処理）
2. **結果分析と統計**

## Valgrindメモリ検査について

### 機能
- **メモリエラー検出**: 不正なメモリアクセス、バッファオーバーフロー
- **メモリリーク検出**: 解放されていないメモリ領域
- **詳細レポート**: エラー発生箇所と原因の特定

### 設定
- minishell実行時のみValgrindを使用
- bashは通常実行（比較用ベースライン）
- エラー終了コード42でメモリエラーを検出

## 安全性について
- すべてのテストはDockerコンテナ内で実行されます
- 60秒のタイムアウトでハングアップを防止
- コンテナは実行後に自動的に削除されます
- ホストシステムへの影響を最小化

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

### Valgrind関連エラーが発生する場合
```bash
# Dockerコンテナ内でValgrindが利用可能か確認
docker run --rm minishell-fuzzer valgrind --version
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

### 継続的なテスト
```bash
# 長時間のテスト（10万回）
./fuzzing_tester/docker_run.sh -n 100000
```

### 結果の詳細分析
```bash
# 統計情報の確認
cat fuzzing_results/stats.json | python3 -m json.tool

# 特定の差異ファイルの詳細確認
cat fuzzing_results/difference_*.json | python3 -m json.tool

# クラッシュファイルの一覧
ls fuzzing_results/crash_*.json

# メモリ問題の一覧
ls fuzzing_results/memory_*.json
```

### Docker環境でのデバッグ
```bash
# コンテナ内に入ってデバッグ
docker run -it --rm \
  -v "$(pwd)/fuzzing_tester:/app/fuzzing_tester" \
  -v "$(pwd)/minishell:/app/minishell" \
  minishell-fuzzer bash

# 手動でfuzzerを実行
python3 /app/fuzzing_tester/fuzzer.py 10
```

## 期待される結果

理想的なminishellは以下の特性を示すべきです：
- **適切なエラーハンドリング**: 危険なコマンドに対して適切なエラーメッセージ
- **メモリ安全性**: Valgrindエラーやリークの発生なし
- **安定した終了**: クラッシュやSEGVの発生なし
- **タイムアウト処理**: 無限ループに対する適切な制御

## 貢献とフィードバック
このツールの改善案やバグ報告は歓迎します。特に以下の点については継続的な改善を行っています：

- より効果的な危険コマンドの生成戦略
- メモリ問題の詳細分析アルゴリズム
- パフォーマンスの最適化
- Docker環境の安定性向上