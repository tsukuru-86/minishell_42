# Minishell

42東京の課題「minishell」のmandatory部分を実装したシンプルなシェル実装です。Bashの一部機能を再現し、42 Norm(Version 3)に準拠。

## 📋 プロジェクト概要

### 開発環境
- **環境**: Ubuntu 22.04 LTS
- **メモリ安全**: Valgrindによるチェック、clangによるサニタイザー

### 主要機能
- **コマンド入力**: readlineライブラリを使用したコマンド入力
- **コマンド実行**: 指定されたビルトインコマンドとPATHから検索した外部コマンドの実行
- **パイプライン処理**、**リダイレクション**: `<`, `>`, `>>`
- **ヒアドキュメント**: `<<`
- **環境変数処理**: 変数の設定、展開（`$VAR`, `$?`）
- **クオート**: シングルクオート(解釈しない)、ダブルクオート(変数を展開)
- **デバッグ機能**: `make debug`でトークン解析・実行フローの可視化

## 🚀 ビルド・実行

```bash
# 通常のビルド
make re

# デバッグ版ビルド（詳細ログ出力）
make debug

make clean  # オブジェクトファイル削除
make fclean # 実行ファイル含む全削除
make re     # libftを含む完全再ビルド

# 実行
./minishell
```

### デバッグ機能デモ
```bash
# デバッグ版をビルドして実行
make debug
echo "ls -la" | ./minishell

# 出力例:
# [DEBUG] NI input: 'ls -la'
# [DEBUG] Token list:
# [DEBUG] Token: 'ls' (type=0)
# [DEBUG] Token: '-la' (type=0)
# [DEBUG] Executing command: /bin/ls
```

## 🛠️ 実装機能

### ビルトインコマンド
- [`echo`](srcs/builtin/builtin_echo.c) (with -n option)
- [`cd`](srcs/builtin/builtin_cd.c) (relative or absolute path)
- [`pwd`](srcs/builtin/builtin_pwd.c)
- [`export`](srcs/builtin/builtin_export.c)
- [`unset`](srcs/builtin/builtin_unset.c)
- [`env`](srcs/builtin/builtin_env.c)
- [`exit`](srcs/builtin/builtin_exit.c)

### クオート処理

```bash
# 隣接する文字列の自動結合
echo "hello"world"123"    # → helloworld123

# 変数展開とクオートの組み合わせ
USER=john
echo "$USER"_test         # → john_test

# リダイレクション
cat <"file1""file2"       # → cat <file1file2
```

詳細は[トークン前処理システム](docs/token_preprocessing.md)を参照してください。

### リダイレクション機能
- **入力リダイレクション**: `< file`
- **出力リダイレクション**: `> file`
- **出力追記リダイレクション**: `>> file`
- **ヒアドキュメント**: `<< delimiter`

### パイプライン処理
- 複数コマンドの入出力を接続: `command1 | command2 | command3`

## 🏗️ アーキテクチャ

### 処理フロー
```
入力文字列
    ↓
[トークナイザー] ← トークンに分割
    ↓
[前処理] ← 変数展開・クオート処理・隣接結合
    ↓
[パーサー] ← コマンド構造に変換
    ↓
[実行] ← パイプライン・リダイレクション実行
```

### 主要コンポーネント

#### 0. 入出力

- 対話・非対話モード対応

#### 1. **トークナイザー** ([`srcs/tokenizer/`](srcs/tokenizer/))
- 入力文字列をトークンに分割
- クオート処理とメタ文字の認識
- 構文チェック機能

#### 2. **前処理システム** ([`srcs/parser/parser_preprocess.c`](srcs/parser/parser_preprocess.c))
- 環境変数展開とトークン整理
- クオート除去と隣接トークン結合

#### 3. **パーサー** ([`srcs/parser/`](srcs/parser/))
- トークンをコマンド構造に変換
- ヒアドキュメント処理
- リダイレクション解析

#### 4. **実行**
- **パイプライン処理** ([`srcs/pipeline/`](srcs/pipeline/))
- **ビルトインコマンド** ([`srcs/builtin/`](srcs/builtin/))
- **外部コマンド実行** ([`srcs/external/`](srcs/external/)): PATH検索とプロセス制御

#### 5. **リダイレクション** ([`srcs/redirect/`](srcs/redirect/))
- ファイルディスクリプタ管理
- 入出力のファイルディスクリプタ設定・復元
- エラーハンドリング

#### 6. **環境変数** ([`srcs/env/`](srcs/env/))
- 環境変数の管理と展開
- `$VAR`、`$?`の処理
- 変数の設定と削除

## 📁 プロジェクト構造

```
minishell_42/
├── README.md               # このファイル
├── Makefile               # ビルド設定（debug ターゲット含む）
├── minishell.h            # メインヘッダーファイル
├── docs/                  # ドキュメント
│   ├── subject-madantory.md    # 課題仕様
│   └── token_preprocessing.md  # 前処理システム詳細
├── srcs/                  # ソースコード
│   ├── main.c             # メインエントリポイント
│   ├── main_loop.c        # メインループ処理
│   ├── builtin/           # 内蔵コマンド
│   │   ├── builtin_commands.h     # 内蔵コマンド定義
│   │   ├── builtin_export.h       # export専用ヘッダー
│   │   ├── builtin_echo.c         # echo実装
│   │   ├── builtin_cd.c           # cd実装
│   │   ├── builtin_export*.c      # export実装（複数ファイル）
│   │   └── ...
│   ├── env/               # 環境変数処理
│   │   ├── env_expand.c           # 変数展開
│   │   ├── env_utils.c            # 環境変数ユーティリティ
│   │   └── ...
│   ├── external/          # 外部コマンド実行
│   │   ├── external.h             # 外部コマンド定義
│   │   ├── external_commands.c    # コマンド実行
│   │   └── ...
│   ├── parser/            # パーサーと前処理
│   │   ├── parser.h               # パーサー定義
│   │   ├── parser.c               # メインパーサー
│   │   ├── parser_preprocess.c    # 前処理システム
│   │   ├── heredoc*.c             # ヒアドキュメント処理
│   │   └── ...
│   ├── pipeline/          # パイプライン処理
│   │   ├── pipeline.h             # パイプライン定義
│   │   ├── pipeline.c             # パイプライン実行
│   │   └── ...
│   ├── redirect/          # リダイレクション
│   │   ├── redirect.h             # リダイレクション定義
│   │   ├── redirect.c             # リダイレクション処理
│   │   └── ...
│   ├── tokenizer/         # トークン化
│   │   ├── tokenizer.h            # トークナイザー定義
│   │   ├── tokenizer.c            # メイントークナイザー
│   │   └── ...
│   └── utils/             # ユーティリティ
│       ├── debug*.c               # デバッグ機能
│       ├── input_utils.h          # 入力処理定義
│       ├── system_limits.h        # システム制限定義
│       └── ...
├── libft/                 # 自作ライブラリ
└── obj/                   # オブジェクトファイル（生成される）
```

## 📋 技術仕様

### コーディング規則 （課題およびNorm準拠）
- **関数制限**: 1ファイル5関数以下、関数25行以内、引数4つ以下
- **グローバル変数**: [`g_signal`](minishell.h:58)のみ許可
- **禁止構文**: for文、do-while文、switch文、goto文禁止
- **許可関数 (下記)**: readline系、標準I/O、プロセス制御、libft関数など
- **インデント**: タブ文字のみ使用
- **命名規則**: snake_case、構造体は`s_`、typedef は`t_`プレフィックス
- **エラー処理**: システムコールは`perror()`、カスタムエラーは統一メッセージ
- **メモリ管理**: リーク防止、適切なリソース解放、適切なエラーハンドリング (SEGV・double freeなどによる不用意な停止の防止)

### 使用可能関数
```c
// readline系
readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history

// 標準I/O・メモリ
printf, malloc, free, write, access, open, read, close

// プロセス制御
fork, wait, waitpid, wait3, wait4, exit, execve

// シグナル処理
signal, sigaction, sigemptyset, sigaddset, kill

// ファイル・ディレクトリ操作
getcwd, chdir, stat, lstat, fstat, unlink, dup, dup2, pipe
opendir, readdir, closedir

// エラー処理・端末制御
strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv
tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

// libft関数全般
```

### デバッグ機能
開発・デバッグ用の詳細ログ出力機能を提供：

```bash
# デバッグ版をビルド
make debug

# デバッグ情報付きで実行
./minishell
# または
echo "ls -la" | ./minishell

# 出力例:
# [DEBUG] NI input: 'ls -la'
# [DEBUG] prepare_input result: 'ls -la'
# [DEBUG] validate_input returned true
# [DEBUG] Token list:
# [DEBUG] Token: 'ls' (type=0)
# [DEBUG] Token: '-la' (type=0)
# [DEBUG] Executing command: /bin/ls
```

**デバッグ情報の内容:**
- 入力処理の流れ（トークン化、検証、実行）
- トークンリストの詳細表示
- コマンド実行時の引数一覧
- 対話・非対話モードの判別
- 各処理段階のステータス

### 実行例
```bash
# 実行ファイルを起動
./minishell

# 基本コマンド例
minishell$ echo "Hello, World!"
Hello, World!

minishell$ export MY_VAR=test
minishell$ echo $MY_VAR
test

minishell$ ls -la | grep ".c" | wc -l
42

minishell$ cat << EOF
> line1
> line2  
> EOF
line1
line2
```

## 🧪 テスト方法

### 基本機能テスト
```bash
# 基本コマンド
echo "test" | cat
pwd && ls -la
export TEST=value && echo $TEST

# パイプラインテスト
ls | grep ".c" | head -5
cat Makefile | wc -l

# リダイレクションテスト
echo "test" > /tmp/test.txt
cat < /tmp/test.txt
echo "append" >> /tmp/test.txt

# クオートテスト  
echo 'hello'"world"
echo "$USER"_test
```

### 品質チェック
```bash
# Norm準拠チェック
norminette *.h srcs/

# コンパイル警告チェック
make cre 2>&1 | grep -i warning

# メモリリークチェック（要valgrind）
valgrind --leak-check=full ./minishell
```

## 🎯 実装のハイライト

### 高度なクオート処理システム
- **隣接文字列結合**: `"hello"world"123"` → `helloworld123`
- **変数展開**: `"$USER"_test` → `john_test`
- **複雑なパターン**: `cat <"file1""file2"` → `cat <file1file2`

### モジュラーアーキテクチャ
- **独立モジュール**: 各機能が独立したモジュールとして実装
- **ヘッダー分離**: モジュール内部関数と公開API の明確な分離
- **エラー処理**: 統一されたエラーハンドリング

### 42 Norm完全準拠
- **厳格な制約**: 1ファイル5関数以下、関数25行以内
- **メモリ管理**: リーク防止とエラー処理
- **構造体設計**: 引数数制限に対応した効率的な設計

## 🎁 プロジェクト要求仕様

### Mandatory 要件（42課題）
1. **プロンプト表示**: 新しいコマンド待機時
2. **履歴機能**: コマンド履歴の保持
3. **実行ファイル検索**: PATH変数基準、相対/絶対パス対応
4. **シグナル処理**: Ctrl-C（新プロンプト）、Ctrl-D（終了）、Ctrl-\（無視）
5. **クオート処理**: シングルクオート（リテラル）、ダブルクオート（変数展開有効）
6. **リダイレクション**: `<`, `>`, `>>`, `<<`の完全実装
7. **パイプライン**: `|`によるコマンド連携
8. **環境変数展開**: `$VAR`、`$?`（前回終了ステータス）
9. **内蔵コマンド**: echo、cd、pwd、export、unset、env、exit

### 品質基準
- **メモリ管理**: readline以外のリーク禁止
- **エラー処理**: 異常終了（SEGV、double free等）禁止
- **互換性**: bashを参考基準とした動作
- **コード品質**: 42 Norm完全準拠

## 🔄 開発ルール

### 42 Norm遵守事項
1. **ファイル構造**: 1ファイル5関数以下、関数25行以内
2. **変数制限**: ローカル変数5つ以下、引数4つ以下
3. **制御構造**: while文のみ使用、for/do-while文禁止
4. **メモリ管理**: リーク防止、適切なエラー処理
5. **コーディングスタイル**: インデントはタブ、明確な命名規則

### エラーハンドリング
- **システムエラー**: `perror("minishell: 操作名")`で統一
- **カスタムエラー**: [`minishell.h`](minishell.h)の定義済みメッセージ使用
- **メモリエラー**: 即座にクリーンアップして終了
- **シグナル処理**: [`g_signal`](minishell.h:58)でのみ状態管理

## 🤝 制限事項

### 未対応機能
- **配列・関数**: Bash配列、関数定義、ワイルドカード展開
- **履歴**: コマンド履歴の永続化は未実装
- **ジョブ制御**: バックグラウンド処理は未対応

### システム制限
- **入力長**: [`ARG_MAX`](srcs/utils/system_limits.h:29)に準拠
- **パス長**: [`PATH_MAX`](srcs/utils/system_limits.h:21)に準拠
- **ファイル名**: [`NAME_MAX`](srcs/utils/system_limits.h:39)に準拠

## 📄 ライセンス

このリポジトリーにある著作物は、私たちが42東京の課題「minishell」の条件を満たすように作成したものであり、42東京およびエコール42の著作物を含みません。私たちに帰属する部分はMITライセンスで提供します。
(C)2025 muiida, tsukuru-86.

---
**最終更新**: 2025年6月25日
