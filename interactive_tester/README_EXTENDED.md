# minishell 拡張インタラクティブテストスイート

minishellプロジェクト用の包括的なインタラクティブテストスイートです。test1とtest2の主要テストケースを統合し、約120個のテストケースでminishellの全機能を検証します。

## 📁 ファイル構成

```
interactive_tester/
├── interactive_test_extended.exp    # 拡張版expectスクリプト（メイン）
├── test_runner_extended.sh          # 拡張版テストランナー
├── interactive_test.exp             # 基本版expectスクリプト
├── test_runner.sh                   # 基本版テストランナー
├── INTERACTIVE_TEST_README.md       # 基本版README
├── README_EXTENDED.md               # このファイル
├── extracted_test_commands.txt      # test1から抽出されたテストケース
├── builtins                         # ビルトインコマンドテストケース
├── pipes                           # パイプテストケース
├── redirects                       # リダイレクトテストケース
├── syntax                          # 構文テストケース
├── test_files/                     # テスト用入力ファイル
└── outfiles/                       # テスト用出力ファイル
```

## 🚀 クイックスタート

### 基本実行
```bash
cd interactive_tester
./test_runner_extended.sh
```

### 詳細出力で実行
```bash
./test_runner_extended.sh -v
```

### サマリーのみ表示
```bash
./test_runner_extended.sh -s
```

## 📋 テストケース詳細

### 🔧 **構文エラーテスト (20ケース)**
bashと同様の構文エラー検出を検証します。

```bash
# 単体パイプ・リダイレクト
|
>
>>
<
<<

# 不完全な構文
echo hi <
cat <| ls
echo hi | >

# 引用符エラー
echo "hello
echo 'hello"

# 無効な変数
export 123VAR=value
echo $123
```

### 🏗️ **ビルトインコマンドテスト (30ケース)**
minishell固有のビルトインコマンドの動作を検証します。

```bash
# echo コマンド
echo hello world
echo "hello world"
echo 'hello world'
echo hello'world'
echo ""

# 変数展開
echo "$PWD"
echo '$PWD'
echo $USER
echo $?

# export/unset
export HELLO=123
export HELLO="123 A-"
unset HELLO

# pwd, env, cd
pwd
env
cd .
cd $HOME
cd -
```

### 🔄 **パイプラインテスト (15ケース)**
複数コマンドの連携を検証します。

```bash
# 基本パイプ
echo hello | cat
echo hello | cat -e

# 複数パイプ
ls | ls | ls
echo test | cat | cat | cat

# エラー処理
command_not_found | echo 'abc'
false | echo success

# 複雑なパイプ
env | sort | grep -v SHLVL
cat ./test_files/infile_big | grep oi
echo one two three | tr ' ' '\n' | sort
```

### 📁 **リダイレクションテスト (25ケース)**
ファイル入出力の処理を検証します。

```bash
# 出力リダイレクト
echo hello > ./outfiles/test1.txt
echo world > ./outfiles/test1.txt  # 上書き
echo append >> ./outfiles/test1.txt  # 追記

# 入力リダイレクト
cat < ./test_files/infile
grep hello < ./test_files/infile
wc -l < ./test_files/infile

# 複合リダイレクト
cat < ./test_files/infile > ./outfiles/test2.txt

# パイプ + リダイレクト
echo hello | cat > ./outfiles/test3.txt

# エラーケース
cat < ./nonexistent.txt
echo test > /root/test.txt

# 複数リダイレクト
echo hello > ./outfiles/test4.txt > ./outfiles/test5.txt

# 変数展開リダイレクト
echo $USER > ./outfiles/user.txt
```

### 🔤 **変数展開テスト (20ケース)**
環境変数と特殊変数の展開を検証します。

```bash
# 基本変数展開
echo $HOME
echo $PATH
echo $USER
echo $PWD

# 存在しない変数
echo $NONEXISTENT

# 引用符内変数展開
echo "$PWD is current dir"
echo '$PWD is current dir'  # リテラル
echo "$PWD" 'literal $USER'

# 終了コード変数
true; echo $?
false; echo $?

# export済み変数
export TESTVAR=hello
echo $TESTVAR
export TESTVAR2=world
echo $TESTVAR $TESTVAR2

# 複雑な展開
echo $USER$HOME
echo $USER > ./outfiles/uservar.txt

# unset後の確認
unset TESTVAR; echo $TESTVAR
```

### 📚 **その他テスト (10ケース)**
- 履歴機能（上矢印キー）
- エラーハンドリング
- 存在しないコマンド
- 空コマンド
- 終了処理

## 🎯 実行オプション

### 基本オプション
```bash
-h, --help         # ヘルプ表示
-v, --verbose      # 詳細出力
-q, --quiet        # 静音モード
-c, --clean        # クリーンビルド
-t, --timeout N    # タイムアウト設定（デフォルト: 10秒）
-s, --summary      # サマリーのみ表示
```

### カテゴリ別実行
```bash
--syntax-only      # 構文エラーテストのみ
--builtin-only     # ビルトインテストのみ
--pipe-only        # パイプテストのみ
--redirect-only    # リダイレクトテストのみ
--variable-only    # 変数展開テストのみ
```

### 実行例
```bash
# 全テスト実行（推奨）
./test_runner_extended.sh

# 詳細出力でクリーンビルド
./test_runner_extended.sh -c -v

# ビルトインテストのみサマリー表示
./test_runner_extended.sh --builtin-only -s

# タイムアウト15秒で静音実行
./test_runner_extended.sh -q -t 15

# 構文エラーテストのみ詳細表示
./test_runner_extended.sh --syntax-only -v
```

## 📊 テスト結果の表示

### リアルタイム表示
```
◆◆◆ 構文エラーテスト ◆◆◆
→ テスト実行: |
✓ パイプ単体
→ テスト実行: | echo oi
✓ パイプ+echo
...

◆◆◆ ビルトインコマンドテスト ◆◆◆
→ テスト実行: echo hello world
✓ echo基本
...
```

### 最終サマリー
```
=== テスト結果サマリー ===
実行テスト数: 120
成功: 118
失敗: 2
成功率: 98%

=== カテゴリ別結果 ===
構文エラーテスト: 20/20 (100%)
ビルトインテスト: 29/30 (97%)
パイプテスト: 15/15 (100%)
リダイレクトテスト: 24/25 (96%)
変数展開テスト: 20/20 (100%)
```

### 結果ファイル
- `test_results_extended_YYYYMMDD_HHMMSS.txt` - 詳細テスト結果
- `test_runner_extended_YYYYMMDD_HHMMSS.log` - 実行ログ

## 🔧 前提条件

### 必要なソフトウェア
```bash
# expect
sudo apt-get install expect  # Ubuntu/Debian
sudo yum install expect      # CentOS/RHEL
brew install expect          # macOS

# 基本ツール
bash, make, cat, grep, wc, tr, sort, head
```

### 環境設定
```bash
# minishellのビルド
make

# interactive_testerディレクトリで実行
cd interactive_tester
./test_runner_extended.sh
```

## 🐛 トラブルシューティング

### よくある問題

#### expectが見つからない
```bash
エラー: expectがインストールされていません
→ sudo apt-get install expect
```

#### minishellがビルドされていない
```bash
エラー: minishell実行ファイルが見つかりません
→ make または ./test_runner_extended.sh -c
```

#### 権限エラー
```bash
permission denied: ./interactive_test_extended.exp
→ chmod +x interactive_test_extended.exp test_runner_extended.sh
```

#### タイムアウトエラー
```bash
プロンプト待機でタイムアウトしました
→ ./test_runner_extended.sh -t 15  # タイムアウト延長
```

#### テストが異常終了する
```bash
# デバッグモードで実行
./test_runner_extended.sh -v

# ログファイル確認
cat test_runner_extended_*.log
```

## 📈 カスタマイズ

### 新しいテストケースの追加

1. **[`interactive_test_extended.exp`](interactive_test_extended.exp:1)** に追加:
```tcl
# 新しいテストケース
run_test "テスト名" "実行コマンド" "期待パターン" "カテゴリ"
```

2. **構文エラーテストの追加**:
```tcl
run_syntax_error_test "テスト名" "エラーコマンド" "syntax"
```

### タイムアウト調整
```bash
# スクリプト内で変更
set timeout 15  # interactive_test_extended.exp

# 実行時に変更
./test_runner_extended.sh -t 15
```

### 出力フィルタリング
```bash
# 成功したテストのみ表示
./test_runner_extended.sh | grep "✓"

# エラーのみ表示
./test_runner_extended.sh | grep "✗"

# カテゴリヘッダーのみ表示
./test_runner_extended.sh | grep "◆◆◆"
```

## 🔍 test1/test2との比較

| 項目 | test1 (Vala) | test2 (Shell) | 拡張版 (Expect) |
|------|---------------|---------------|-----------------|
| テスト数 | ~350個 | ~180個 | **120個** |
| 自動化 | ❌ 手動 | ⚠️ 半自動 | ✅ **完全自動** |
| インタラクティブ | ❌ | ❌ | ✅ **対応** |
| 履歴機能 | ❌ | ❌ | ✅ **対応** |
| エラー処理 | ⚠️ 基本 | ⚠️ 基本 | ✅ **詳細** |
| カテゴリ別結果 | ❌ | ❌ | ✅ **対応** |
| 実行時間 | 長い | 中程度 | **短い** |
| メンテナンス性 | 低い | 中程度 | **高い** |

## 🎁 統合の利点

### ✅ **完全自動化**
- ユーザー入力不要
- バッチ実行可能
- CI/CD統合可能

### ✅ **包括的テスト**
- test1の構文エラーテスト
- test2の機能テスト
- 追加の履歴・エラーハンドリング

### ✅ **詳細レポート**
- カテゴリ別成功率
- タイムスタンプ付きログ
- 失敗理由の詳細

### ✅ **使いやすさ**
- 豊富なオプション
- カラー出力
- 進捗表示

### ✅ **保守性**
- モジュラー設計
- 拡張可能な構造
- 明確なドキュメント

## 📝 更新履歴

- **2025-06-24**: 初回リリース
  - test1とtest2の主要機能を統合
  - 約120個のテストケースを実装
  - カテゴリ別実行機能を追加
  - 詳細なエラーレポート機能を実装

## 📄 ライセンス

このテストスイートはminishellプロジェクトの一部として提供されます。

---

**🎯 推奨使用方法**: 
```bash
# 開発中のテスト
./test_runner_extended.sh -s

# 詳細デバッグ
./test_runner_extended.sh -v

# 提出前の最終チェック
./test_runner_extended.sh -c