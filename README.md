w# Minishell

42東京の課題として開発されたシンプルなシェル実装です。Bashの基本機能を再現し、コマンドライン処理、パイプライン、リダイレクション、環境変数処理などの機能を提供します。

## 特徴

### 基本機能
- **インタラクティブシェル**: readlineライブラリを使用したコマンド入力
- **コマンド実行**: 内蔵コマンドと外部コマンドの実行
- **パイプライン処理**: 複数コマンドの連携実行
- **リダイレクション**: 入力・出力の方向転換（`<`, `>`, `>>`）
- **ヒアドキュメント**: `<<`によるインライン入力
- **環境変数**: 変数の設定、参照、展開

### 内蔵コマンド
- `echo` (with -n option)
- `cd` (relative or absolute path)
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

### 高度なクオート処理 🆕
最新バージョンでは、従来のシェルでは処理が困難だった複雑なクオートパターンに対応しています：

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

この機能により、Bashとの互換性が大幅に向上し、より自然なコマンドライン体験を提供します。

## アーキテクチャ

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

- **トークナイザー** (`srcs/tokenizer/`): 入力文字列をトークンに分割
- **前処理システム** (`srcs/parser/parser_preprocess.c`): 高度なクオート処理
- **パーサー** (`srcs/parser/`): トークンをコマンド構造に変換
- **実行エンジン** (`srcs/pipeline/`, `srcs/external/`): コマンド実行とプロセス管理
- **リダイレクション** (`srcs/redirect/`): ファイルディスクリプタ管理
- **内蔵コマンド** (`srcs/builtin/`): シェル組み込み機能

## ビルド方法

### 必要な環境
- GCC または Clang
- Make
- readline ライブラリ

### コンパイル
```bash
make
```

### クリーンビルド
```bash
make clean  # オブジェクトファイル削除
make fclean # 実行ファイル含む全削除
make re     # 完全再ビルド
```

## 使用方法

### 基本的な使用例

```bash
# 実行ファイルを起動
./minishell

# 基本コマンド
minishell$ echo "Hello, World!"
Hello, World!

# パイプライン
minishell$ ls -la | grep ".c" | wc -l

# リダイレクション
minishell$ echo "test" > output.txt
minishell$ cat < output.txt

# 環境変数
minishell$ export MY_VAR=hello
minishell$ echo $MY_VAR
hello

# ヒアドキュメント
minishell$ cat << EOF
> line1
> line2
> EOF
line1
line2
```

### 高度なクオート処理の例

```bash
# 隣接文字列の結合
minishell$ echo 'hello'"world"
helloworld

# 変数展開との組み合わせ
minishell$ NAME=john
minishell$ echo "Hello, "$NAME"!"
Hello, john!

# 複雑なファイル名指定
minishell$ touch "test""file"".txt"
# → testfile.txt が作成される
```

## プロジェクト構造

```
minishell_42/
├── Makefile              # ビルド設定
├── minishell.h          # メインヘッダーファイル
├── README.md            # このファイル
├── docs/                # ドキュメント
│   └── token_preprocessing.md  # 前処理システム詳細
├── libft/               # 自作ライブラリ
├── srcs/                # ソースコード
│   ├── main.c
│   ├── builtin/         # 内蔵コマンド
│   ├── external/        # 外部コマンド実行
│   ├── parser/          # パーサーと前処理
│   ├── pipeline/        # パイプライン処理
│   ├── redirect/        # リダイレクション
│   ├── tokenizer/       # トークン化
│   └── utils/           # ユーティリティ
└── test/                # テストファイル
```

## 開発情報

### 技術的詳細

前処理システムの詳細な実装については、[`docs/token_preprocessing.md`](docs/token_preprocessing.md)を参照してください。

### コーディング規則
- 42 Norminette準拠
- 関数は25行以内
- 適切なエラーハンドリング
- メモリリークの防止

### テスト
```bash
# 基本機能テスト
./minishell < test/basic_commands.txt

# クオート処理テスト  
./minishell < test/quote_tests.txt

# パイプラインテスト
./minishell < test/pipeline_tests.txt
```

## 制限事項

- 一部のBash高度機能（配列、関数定義等）は未対応
- コマンド履歴の永続化は未実装
- ジョブ制御機能は未対応
- ワイルドカード展開は部分対応

## 貢献

バグ報告や機能提案は歓迎します。プルリクエストを送信前に：

1. Norminetteチェックの実行
2. 既存テストの通過確認
3. 新機能に対するテスト追加

## ライセンス

このプロジェクトは42東京の課題として開発されました。教育目的での使用を想定しています。

## 更新履歴

### v1.1.0 (2025/06/05)
- 🆕 高度なクオート処理システムの実装
- 隣接文字列の自動結合機能
- 変数展開とクオートの統合処理
- パフォーマンス向上とメモリ効率化

### v1.0.0 (2025/05/01)
- 基本的なシェル機能の実装
- 内蔵コマンドの追加
- パイプラインとリダイレクション対応

---
**開発チーム**: 42東京 minishell プロジェクト  
**最終更新**: 2025年6月5日
