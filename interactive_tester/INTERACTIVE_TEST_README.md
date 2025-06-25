# minishell インタラクティブテスト

minishellプロジェクト用のインタラクティブテストスイートです。expectスクリプトを使用して自動的にminishellとの対話テストを実行します。

## 作成されたファイル

- [`interactive_test.exp`](interactive_test.exp:1) - メインのexpectスクリプト
- [`test_runner.sh`](test_runner.sh:1) - テスト実行用シェルスクリプト
- [`INTERACTIVE_TEST_README.md`](INTERACTIVE_TEST_README.md:1) - このドキュメント

## 前提条件

### 必要なソフトウェア
- `expect` パッケージがインストールされていること
- `bash` シェル
- `make` コマンド

### expectのインストール方法

```bash
# Ubuntu/Debian
sudo apt-get install expect

# CentOS/RHEL/Fedora
sudo yum install expect
# または
sudo dnf install expect

# macOS (Homebrew)
brew install expect
```

## 使用方法

### 基本的な使用方法

```bash
# テストランナーを使用（推奨）
./test_runner.sh

# expectスクリプトを直接実行
./interactive_test.exp
```

### テストランナーのオプション

```bash
# ヘルプ表示
./test_runner.sh -h

# 詳細出力で実行
./test_runner.sh -v

# 静かに実行（エラー以外非表示）
./test_runner.sh -q

# クリーンビルド後にテスト実行
./test_runner.sh -c

# タイムアウト時間を10秒に設定
./test_runner.sh -t 10

# オプションの組み合わせ
./test_runner.sh -c -v -t 15
```

## テスト内容

### 実行されるテストケース

1. **基本コマンドテスト**
   - [`echo hello`](interactive_test.exp:83) - エコーコマンド
   - [`pwd`](interactive_test.exp:86) - 現在ディレクトリ表示  
   - [`ls`](interactive_test.exp:89) - ファイル一覧表示

2. **ビルトインコマンドテスト**
   - [`env`](interactive_test.exp:94) - 環境変数表示
   - [`export TEST_VAR=hello`](interactive_test.exp:97) - 環境変数設定
   - [`export`](interactive_test.exp:100) - 設定確認
   - [`unset TEST_VAR`](interactive_test.exp:103) - 環境変数削除
   - [`cd .`](interactive_test.exp:106) - ディレクトリ変更

3. **パイプテスト**
   - [`echo hello | cat`](interactive_test.exp:111) - 基本的なパイプ

4. **リダイレクトテスト**
   - [`echo test > test_output.txt`](interactive_test.exp:116) - 出力リダイレクト
   - [`cat < test_output.txt`](interactive_test.exp:119) - 入力リダイレクト
   - [`rm test_output.txt`](interactive_test.exp:122) - テストファイル削除

5. **履歴機能テスト**
   - [`echo history_test`](interactive_test.exp:127) - コマンド実行
   - 上矢印キーでの履歴呼び出し

6. **エラーハンドリングテスト**
   - [`nonexistent_command`](interactive_test.exp:143) - 存在しないコマンド
   - [`echo |`](interactive_test.exp:146) - 無効な構文

7. **終了テスト**
   - [`exit`](interactive_test.exp:151) - 正常終了

### テスト設定

- **タイムアウト時間**: デフォルト5秒（変更可能）
- **プロンプト**: `minishell> ` を待機
- **結果保存**: タイムスタンプ付きのテスト結果ファイルを自動生成

## 出力とログ

### テスト実行時の出力
- カラー表示でテスト進行状況を表示
- 成功/失敗の即座な判定
- 詳細なエラー情報

### 生成されるファイル
- `test_results_YYYYMMDD_HHMMSS.txt` - テスト結果詳細
- `test_runner_YYYYMMDD_HHMMSS.log` - 実行ログ（test_runner.sh使用時）

## トラブルシューティング

### よくある問題と解決方法

1. **expectが見つからない**
   ```
   エラー: expectがインストールされていません
   ```
   → expectパッケージをインストールしてください

2. **minishellがビルドされていない**
   ```
   エラー: minishell実行ファイルが見つかりません
   ```
   → `make` コマンドでビルドするか、`./test_runner.sh -c` を実行

3. **権限エラー**
   ```
   permission denied: ./interactive_test.exp
   ```
   → `chmod +x interactive_test.exp test_runner.sh` で実行権限を付与

4. **タイムアウトエラー**
   ```
   プロンプト待機でタイムアウトしました
   ```
   → `-t` オプションでタイムアウト時間を延長

### デバッグモード
詳細な実行ログが必要な場合：
```bash
./test_runner.sh -v
```

## カスタマイズ

### テストケースの追加
[`interactive_test.exp`](interactive_test.exp:75)ファイルの該当セクションに新しいテストケースを追加できます：

```tcl
# 新しいテストケースの例
run_test "新しいテスト名" "実行するコマンド" "期待する出力パターン"
```

### タイムアウト時間の調整
- デフォルト: 5秒
- スクリプト内で変更: [`interactive_test.exp`](interactive_test.exp:7)の `set timeout 5` を編集
- 実行時に変更: `./test_runner.sh -t 10`

## 注意事項

1. **実行環境**: minishellプロジェクトのルートディレクトリで実行してください
2. **一時ファイル**: テスト中に一時ファイルが作成されますが、自動的に削除されます
3. **インタラクティブ性**: このテストはminishellの対話機能をテストするため、バックグラウンドでは実行できません

## テスト結果の解釈

### 成功例
```
✓ echo hello
✓ pwd
✓ ls
...
実行テスト数: 15
成功: 15
失敗: 0
成功率: 100%
```

### 失敗例
```
✗ echo hello
  タイムアウト
✓ pwd
...
実行テスト数: 15
成功: 14
失敗: 1
成功率: 93%
```

失敗したテストの詳細は生成されるテスト結果ファイルで確認できます。

## ライセンス

このテストスイートはminishellプロジェクトの一部として提供されます。