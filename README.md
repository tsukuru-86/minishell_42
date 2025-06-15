# Minishell

42東京の課題として開発されたシンプルなシェル実装です。Bashの基本機能を再現し、継続的な改善により**高品質なシェル実装**を達成しています。

## 📋 プロジェクト概要

### 基本情報
- **プロジェクト名**: minishell
- **言語**: C言語
- **対象課題**: 42 School mandatory project
- **準拠規格**: 42 Norm Version 3
- **テスト成功率**:
  - **test2**: 146/146 (100%) ✅ 完璧なスコア
  - **test1**: 243/298 (81.5%) 📈 優良なスコア
- **メモリ安全**: Valgrindリークなし確認済み ✅

### 主要機能
- **インタラクティブシェル**: readlineライブラリを使用したコマンド入力
- **コマンド実行**: 内蔵コマンドと外部コマンドの実行
- **パイプライン処理**: 複数コマンドの連携実行
- **リダイレクション**: 入力・出力の方向転換（`<`, `>`, `>>`, `<<`）
- **環境変数処理**: 変数の設定、参照、展開（`$VAR`, `$?`）
- **高度なクオート処理**: 複雑なクオートパターンの正確な解析
- **デバッグ機能**: `make debug`でトークン解析・実行フローの可視化

## 🚀 クイックスタート

### ビルド・実行
```bash
# 通常のビルド
make

# デバッグ版ビルド（詳細ログ出力）
make debug

# 実行
./minishell
```

### デバッグ機能デモ
```bash
# デバッグ版をビルドして実行
make debug
echo "ls -la" | ./minishell

# 出力例:
# [DEBUG] Non-inter input: 'ls -la'
# [DEBUG] Token list:
# [DEBUG] Token: 'ls' (type=0)
# [DEBUG] Token: '-la' (type=0)
# [DEBUG] Executing command: /bin/ls
# [DEBUG] Command arguments:
# [DEBUG] args[0]: 'ls'
# [DEBUG] args[1]: '-la'
```

## 🛠️ 実装機能

### 内蔵コマンド
- `echo` (with -n option)
- `cd` (relative or absolute path)
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

### 高度なクオート処理システム
```bash
# 隣接する文字列の自動結合
echo "hello"world"123"    # → helloworld123

# 変数展開とクオートの組み合わせ
USER=john
echo "$USER"_test         # → john_test

# 複雑なリダイレクション
cat <"file1""file2"       # → cat <file1file2
```

### リダイレクション機能
- **入力リダイレクション**: `< file`
- **出力リダイレクション**: `> file`
- **追記リダイレクション**: `>> file`
- **ヒアドキュメント**: `<< delimiter`

### パイプライン処理
- 複数コマンドの連携実行: `command1 | command2 | command3`
- プロセス間通信の適切な管理
- エラー状態の伝播処理

## 📊 品質指標

### テスト結果
- **総テスト数**: 146
- **成功**: 146 ✅
- **失敗**: 0
- **成功率**: 100%

### 42 Norm準拠
- ✅ 1ファイル5関数以下
- ✅ 関数25行以内、引数4つ以下
- ✅ グローバル変数 `g_signal` のみ
- ✅ for文・switch文禁止遵守

### メモリ安全性
- ✅ Valgrind全テストでリークなし
- ✅ 適切なエラーハンドリング
- ✅ SEGV・double free防止

## 📁 プロジェクト構造

```
minishell_42/
├── README.md            # このファイル（プロジェクト概要）
├── DEVELOPMENT.md       # 開発者向け詳細ドキュメント
├── Makefile            # ビルド設定（debug ターゲット含む）
├── minishell.h         # メインヘッダーファイル
├── srcs/               # ソースコード
│   ├── main.c          # メインエントリポイント
│   ├── main_loop.c     # メインループ処理
│   ├── builtin/        # 内蔵コマンド
│   ├── env/            # 環境変数処理
│   ├── external/       # 外部コマンド実行
│   ├── parser/         # パーサーと前処理
│   ├── pipeline/       # パイプライン処理
│   ├── redirect/       # リダイレクション
│   ├── tokenizer/      # トークン化
│   └── utils/          # ユーティリティ
│       ├── debug.c     # デバッグ機能
│       └── empty_input_handler.c  # 空入力処理
└── libft/              # 自作ライブラリ
```

## 🎯 実装のハイライト

### 最新の修正内容（2025年6月15日）
- ✅ **`$"`特殊構文修正**: `printf $"  $USER  a"`問題を解決
- ✅ **ビルトイン関数識別精度向上**: 空文字列コマンドの正確な処理
- ✅ **環境変数処理改善**: 特殊ケースでの`$`文字処理を最適化
- ✅ **test1スコア向上**: 239/298 (80.2%) → 243/298 (81.5%)
- ✅ **test2完璧維持**: 146/146 (100%) の高品質を継続

### 前回の修正内容（2025年6月14日）
- ✅ **デバッグ機能の実装**: `make debug`でトークン解析・実行フローの可視化
- ✅ **exit code修正**: 実行権限なし（126）、コマンド未発見（127）の適切な処理
- ✅ **空コマンド処理**: `"aaa"`, `$`, `$?`の正しいエラーハンドリング
- ✅ **$EMPTY修正**: 未定義変数展開時のexit code 0対応（bash互換）

### 技術的特徴
- **Norm完全準拠**: 42 Schoolの厳格なコーディング規則に100%準拠
- **メモリ安全**: 商用レベルのメモリ管理とリーク防止
- **デバッグ支援**: 開発・学習用の詳細ログ機能
- **高い互換性**: bashを参考基準とした動作実装

## 📈 開発状況

- **v1.5.0 (2025/06/15)**: 継続的改善により品質向上、test1スコア81.5%達成
- **v1.4.0 (2025/06/14)**: `$EMPTY`修正、テスト成功率100%達成
- **v1.3.0 (2025/06/14)**: デバッグ機能追加、テスト成功率99.3%達成
- **v1.2.0 (2025/06/14)**: ドキュメント統合・整理
- **v1.1.0 (2025/06/05)**: 高度クオート処理システム実装
- **v1.0.0 (2025/05/01)**: 基本シェル機能実装

## 🤝 制限事項

- **未対応機能**: Bash配列、関数定義、ワイルドカード展開
- **履歴**: コマンド履歴の永続化は未実装
- **ジョブ制御**: バックグラウンド処理は未対応

## 📄 追加情報

- **詳細技術情報**: [`DEVELOPMENT.md`](DEVELOPMENT.md) を参照
- **Norm規則**: [`en.norm.tex`](en.norm.tex) を参照
- **ライセンス**: 42東京の課題として開発、教育目的での使用を想定

---
**開発チーム**: 42東京 minishell プロジェクト  
**最終更新**: 2025年6月14日

## 🏗️ アーキテクチャ

### 処理フロー
```
入力文字列
    ↓
[トークナイザー] ← トークンに分割
    ↓
[前処理システム] ← 変数展開・クオート処理・隣接結合
    ↓
[パーサー] ← コマンド構造に変換
    ↓
[実行エンジン] ← パイプライン・リダイレクション実行
```

### 主要コンポーネント
- **トークナイザー** ([`srcs/tokenizer/`](srcs/tokenizer/)): 入力文字列をトークンに分割
- **前処理システム** ([`srcs/parser/parser_preprocess.c`](srcs/parser/parser_preprocess.c)): 高度なクオート処理
- **パーサー** ([`srcs/parser/`](srcs/parser/)): トークンをコマンド構造に変換
- **実行エンジン** ([`srcs/pipeline/`](srcs/pipeline/), [`srcs/external/`](srcs/external/)): コマンド実行とプロセス管理
- **リダイレクション** ([`srcs/redirect/`](srcs/redirect/)): ファイルディスクリプタ管理
- **内蔵コマンド** ([`srcs/builtin/`](srcs/builtin/)): シェル組み込み機能

## 🛠️ 実装機能

### 内蔵コマンド
- [`echo`](srcs/builtin/builtin_echo.c) (with -n option)
- [`cd`](srcs/builtin/builtin_cd.c) (relative or absolute path)
- [`pwd`](srcs/builtin/builtin_pwd.c)
- [`export`](srcs/builtin/builtin_export.c)
- [`unset`](srcs/builtin/builtin_unset.c)
- [`env`](srcs/builtin/builtin_env.c)
- [`exit`](srcs/builtin/builtin_exit.c)

### 高度なクオート処理システム 🆕
従来のシェルでは処理が困難だった複雑なクオートパターンに対応：

```bash
# 隣接する文字列の自動結合
$ echo "hello"world"123"
helloworld123

# 変数展開とクオートの組み合わせ  
$ USER=john
$ echo "$USER"_test
john_test

# 複雑なリダイレクション
$ cat <"file1""file2"
# → cat <file1file2 として処理
```

詳細は[トークン前処理システム](docs/token_preprocessing.md)を参照してください。

### リダイレクション機能
- **入力リダイレクション**: `< file`
- **出力リダイレクション**: `> file`
- **追記リダイレクション**: `>> file`
- **ヒアドキュメント**: `<< delimiter`

### パイプライン処理
- 複数コマンドの連携実行: `command1 | command2 | command3`
- プロセス間通信の適切な管理
- エラー状態の伝播処理

## 📋 技術仕様

### 開発制約（42 Norm準拠）
- **関数制限**: 1ファイル5関数以下、関数25行以内、引数4つ以下
- **グローバル変数**: [`g_signal`](minishell.h:45)のみ許可
- **禁止構文**: for文、do-while文、switch文、goto文禁止
- **許可関数**: readline系、標準I/O、プロセス制御、libft関数のみ

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

### コーディング規則（簡潔版）
- **インデント**: タブ文字のみ使用
- **命名規則**: snake_case、構造体は`s_`、typedef は`t_`プレフィックス
- **エラー処理**: システムコールは`perror()`、カスタムエラーは統一メッセージ
- **メモリ管理**: 完全なリーク防止、適切なリソース解放

## 🚀 ビルドと実行

### 必要な環境
- **コンパイラ**: Clang
- **ビルドツール**: Make
- **ライブラリ**: readline library
- **OS**: Linux

### ビルド手順
```bash
# 通常のビルド（リリース版）
make        # または make all

# デバッグビルド（詳細ログ出力付き）
make debug  # -DDEBUG フラグ付きでコンパイル

# クリーンビルド
make cre    # 通常のビルドclean + all
make clean  # オブジェクトファイル削除
make fclean # 実行ファイル含む全削除
make re     # libftを含む完全再ビルド
```

### デバッグ機能 🆕
開発・デバッグ用の詳細ログ出力機能を提供：

```bash
# デバッグ版をビルド
make debug

# デバッグ情報付きで実行
./minishell
# または
echo "ls -la" | ./minishell

# 出力例:
# [DEBUG] Non-inter input: 'ls -la'
# [DEBUG] prepare_input result: 'ls -la'
# [DEBUG] validate_input returned true
# [DEBUG] Token list:
# [DEBUG] Token: 'ls' (type=0)
# [DEBUG] Token: '-la' (type=0)
# [DEBUG] Executing command: /bin/ls
# [DEBUG] Command arguments:
# [DEBUG] args[0]: 'ls'
# [DEBUG] args[1]: '-la'
```

**デバッグ情報の内容:**
- 入力処理の流れ（トークン化、検証、実行）
- トークンリストの詳細表示
- コマンド実行時の引数一覧
- 対話・非対話モードの判別
- 各処理段階のステータス

### 実行方法
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

# デバッグ機能のデモ
./minishell  # デバッグ版で実行
minishell$ echo test
[DEBUG] Interactive input: 'echo test'
[DEBUG] prepare_input result: 'echo test'
[DEBUG] validate_input returned true
[DEBUG] Token list:
[DEBUG] Token: 'echo' (type=0)
[DEBUG] Token: 'test' (type=0)
[DEBUG] Executing builtin: echo
[DEBUG] Command arguments:
[DEBUG] args[0]: 'echo'
[DEBUG] args[1]: 'test'
test
[DEBUG] Status: 0
```

## 📁 プロジェクト構造

```
minishelどうで
├── Makefile              # ビルド設定
├── minishell.h          # メインヘッダーファイル  
├── README.md            # このファイル
├── docs/                # ドキュメント
│   └── token_preprocessing.md  # 前処理システム詳細
├── libft/               # 自作ライブラリ
├── srcs/                # ソースコード
│   ├── main.c           # メインエントリポイント
│   ├── main_loop.c      # メインループ処理
│   ├── builtin/         # 内蔵コマンド（echo, cd, export等）
│   ├── env/             # 環境変数処理
│   ├── external/        # 外部コマンド実行
│   ├── parser/          # パーサーと前処理
│   ├── pipeline/        # パイプライン処理
│   ├── redirect/        # リダイレクション
│   ├── tokenizer/       # トークン化
│   └── utils/           # ユーティリティ
│       ├── debug.c      # デバッグ機能（make debug用）
│       ├── empty_input_handler.c  # 空入力処理
│       └── ...          # その他ユーティリティ
└── obj/                 # オブジェクトファイル（生成される）
```

### 主要ファイル
- **設定**: [`Makefile`](Makefile), [`minishell.h`](minishell.h)
- **コア**: [`srcs/main.c`](srcs/main.c), [`srcs/main_loop.c`](srcs/main_loop.c)
- **処理系**: [`srcs/tokenizer/tokenizer.c`](srcs/tokenizer/tokenizer.c), [`srcs/parser/parser.c`](srcs/parser/parser.c)
- **実行**: [`srcs/pipeline/pipeline.c`](srcs/pipeline/pipeline.c), [`srcs/external/external_commands.c`](srcs/external/external_commands.c)

## 🧪 テスト方法

### 基本機能テスト

- `cd minishell_tester&&./tester&&cd ..`
<!-- - `cd minishell_tester-nda-cunh/&&./tester&&cd ..`(ただしヒアドキュメントは上手く動かないので無視していい) -->

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

## 🔄 開発ルール

### 42 Norm遵守事項
1. **ファイル構造**: 1ファイル5関数以下、関数25行以内
2. **変数制限**: ローカル変数5つ以下、引数4つ以下
3. **制御構造**: while文のみ使用、for/do-while文禁止
4. **メモリ管理**: リーク防止、適切なエラー処理
5. **コーディングスタイル**: インデントはタブ、明確な命名規則

### エラーハンドリング
- **システムエラー**: `perror("minishell: 操作名")`で統一
- **カスタムエラー**: [`minishell.h`](minishell.h:34-43)の定義済みメッセージ使用
- **メモリエラー**: 即座にクリーンアップして終了
- **シグナル処理**: [`g_signal`](minishell.h:45)でのみ状態管理

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

## 📈 更新履歴

### v1.5.0 (2025/06/15) - 継続的改善版
- 🔧 `$"`特殊構文処理の修正
- 🎯 ビルトイン関数識別精度の向上
- 📈 test1スコア 239→243問 (+4問改善)
- ✅ test2完璧スコア維持 (146/146)
- 🚀 継続的な品質向上実現

### v1.3.0 (2025/06/14) - デバッグ機能追加版
- 🔍 デバッグ機能の実装（`make debug`コマンド）
- 📊 トークン解析とコマンド実行の可視化
- 🎯 開発・デバッグ効率の大幅向上
- ✅ 42 Norm準拠のデバッグシステム構築

### v1.2.0 (2025/06/14) - 統合版
- 📝 包括的なREADME.mdの作成
- 📋 既存ドキュメントの統合（トークン処理、バグ修正履歴、開発ルール）
- 🔧 プロジェクト構造の明確化
- 📖 開発者向け情報の整理

### v1.1.0 (2025/06/05) - 高度機能実装
- 🆕 高度なクオート処理システムの実装
- 🔄 隣接文字列の自動結合機能
- 🔀 変数展開とクオートの統合処理
- ⚡ パフォーマンス向上とメモリ効率化

### v1.0.0 (2025/05/01) - 基本版
- 🏗️ 基本的なシェル機能の実装
- 🔧 内蔵コマンドの追加
- 🚰 パイプラインとリダイレクション対応

## 🤝 制限事項

- **未対応機能**: Bash配列、関数定義、ワイルドカード展開
- **履歴**: コマンド履歴の永続化は未実装
- **ジョブ制御**: バックグラウンド処理は未対応
- **高度な展開**: ブレース展開、プロセス置換は未対応

## 📄 ライセンス

このプロジェクトは42東京の課題として開発されました。教育目的での使用を想定しています。

---
**開発チーム**: 42東京 minishell プロジェクト  
**最終更新**: 2025年6月14日  
**ドキュメント統合**: v1.2.0
