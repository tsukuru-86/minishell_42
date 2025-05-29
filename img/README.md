# minishell 関数呼び出し関係図

このディレクトリには、minishellプロジェクトの関数呼び出し関係を視覚化したMermaid図とSVGファイルが含まれています。

## ファイル一覧

### SVG図（完成品）
- **[overview_flow.svg](overview_flow.svg)** - 概要版：メイン処理フローの可視化
- **[detailed_flow.svg](detailed_flow.svg)** - 詳細版：完全な関数呼び出し関係
- **[memory_critical.svg](memory_critical.svg)** - メモリ管理版：メモリリーク分析用

### Mermaidソースファイル
- **[overview_flow.mmd](overview_flow.mmd)** - 概要版のソース
- **[detailed_flow.mmd](detailed_flow.mmd)** - 詳細版のソース  
- **[memory_critical.mmd](memory_critical.mmd)** - メモリ管理版のソース

## 図の説明

### 1. 概要版 (overview_flow.svg)
- **目的**: プロジェクトの基本的な処理フローを理解する
- **対象**: 新規開発者、アーキテクチャ理解
- **内容**: main → tokenizer → parser → utils → builtin/external/pipeline

### 2. 詳細版 (detailed_flow.svg)
- **目的**: 全関数の呼び出し関係を詳細に把握する
- **対象**: デバッグ、コードレビュー、リファクタリング
- **内容**: 全モジュールの関数間依存関係、メモリ管理関数を含む

### 3. メモリ管理版 (memory_critical.svg)
- **目的**: メモリリーク分析とデバッグ
- **対象**: valgrindテスト、メモリ安全性確保
- **内容**: 
  - 🔴 メモリ確保関数：`create_command()`, `tokenize()`, `get_builtin_name()`等
  - 🟢 メモリ解放関数：`free_command()`, `free_tokens()`, `free(builtins)`等
  - create/freeペアの対応関係

## 使用方法

### デバッグ時
```bash
# 関数呼び出しの流れを追跡
# detailed_flow.svg を参照
```

### メモリリーク調査時
```bash
# valgrindテスト実行
./test_valgrind.sh

# memory_critical.svg を参照して
# create/free ペアが正しく呼ばれているか確認
```

### コードレビュー時
```bash
# overview_flow.svg でアーキテクチャ理解
# detailed_flow.svg でモジュール間依存関係確認
```

## メモリ管理のペア対応

| 確保関数 | 解放関数 | 注意点 |
|----------|----------|--------|
| `tokenize()` | `free_tokens()` | トークンチェーンの全解放 |
| `create_command()` | `free_command()` | 引数配列も含む |
| `create_env_list()` | `free_env_list()` | 環境変数リスト |
| `create_redirect()` | `free_redirect()` | リダイレクト構造体 |
| `get_builtin_name()` | `free(builtins)` | 動的確保された配列 |
| `init_heredoc()` | `cleanup_heredoc()` | 一時ファイルも削除 |

## 更新方法

図を更新する場合：

1. `.mmd`ファイルを編集
2. SVGを再生成：
```bash
mmdc -i img/filename.mmd -o img/filename.svg
```

## 技術仕様

- **制約遵守**: for文・do-while文禁止、while文のみ使用
- **メモリ管理**: 関数内static変数を活用
- **コーディング規約**: タブインデント、42 Norm準拠
- **Subject準拠**: subject-madantory.mdの必須機能のみ実装